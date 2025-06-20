#include "mizu/core/audio_mgr.hpp"
#include "mizu/core/log.hpp"
#include "mizu/core/payloads.hpp"
#include "mizu/util/platform.hpp"

#ifndef AL_SOFT_hold_on_disconnect
#define AL_SOFT_hold_on_disconnect
#define AL_STOP_SOURCES_ON_DISCONNECT_SOFT 0x19AB
#endif

namespace mizu {
bool check_al_errors_();
bool check_alc_errors_(ALCdevice *device);
void al_event_callback_(
        ALenum eventType, ALuint object, ALuint param, ALsizei length, const ALchar *message, void *userParam) noexcept;

void alc_event_callback_(
        ALCenum eventType,
        ALCenum deviceType,
        ALCdevice *device,
        ALCsizei length,
        const ALCchar *message,
        void *userParam) noexcept;
const char *FormatName(ALenum format);
Sound load_sound_impl(const std::filesystem::path &path);

AudioMgr::AudioMgr(CallbackMgr &callbacks)
    : callbacks_(callbacks) {
    register_callbacks_();

    device_ = alcOpenDevice(nullptr);
    if (!device_ || !check_alc_errors_(device_)) {
        MIZU_LOG_ERROR("Failed to open OpenAL device!");
        return;
    }

    ctx_ = alcCreateContext(device_, nullptr);
    if (!ctx_ || !check_alc_errors_(device_)) {
        if (ctx_)
            alcDestroyContext(ctx_);
        MIZU_LOG_ERROR("Failed to create OpenAL context");
        return;
    }

    bool context_current_ok = alcMakeContextCurrent(ctx_);
    if (!context_current_ok || !check_alc_errors_(device_)) {
        alcDestroyContext(ctx_);
        alcCloseDevice(device_);
        MIZU_LOG_ERROR("Failed to make OpenAL context current");
        return;
    }
    MIZU_LOG_DEBUG(
            "Initialized OpenAL v{}, vendor: {}, renderer: {}",
            alGetString(AL_VERSION),
            alGetString(AL_VENDOR),
            alGetString(AL_RENDERER));

    alDisable(AL_STOP_SOURCES_ON_DISCONNECT_SOFT);
    check_al_errors_();

    if (alIsExtensionPresent("AL_SOFT_events")) {
        alEventControlSOFT = reinterpret_cast<LPALEVENTCONTROLSOFT>(alGetProcAddress("alEventControlSOFT"));
        alEventCallbackSOFT = reinterpret_cast<LPALEVENTCALLBACKSOFT>(alGetProcAddress("alEventCallbackSOFT"));

        constexpr ALenum types[3] = {
                AL_EVENT_TYPE_BUFFER_COMPLETED_SOFT,
                AL_EVENT_TYPE_SOURCE_STATE_CHANGED_SOFT,
                AL_EVENT_TYPE_DISCONNECTED_SOFT};
        alEventControlSOFT(sizeof(types) / sizeof(ALenum), types, AL_TRUE);
        alEventCallbackSOFT(al_event_callback_, static_cast<void *>(&callbacks_));
    } else {
        MIZU_LOG_WARN("AL_SOFT_events not supported");
    }

    if (alcIsExtensionPresent(device_, "ALC_SOFT_system_events")) {
        alcEventIsSupportedSOFT =
                reinterpret_cast<LPALCEVENTISSUPPORTEDSOFT>(alcGetProcAddress(device_, "alcEventIsSupportedSOFT"));
        alcEventControlSOFT =
                reinterpret_cast<LPALCEVENTCONTROLSOFT>(alcGetProcAddress(device_, "alcEventControlSOFT"));
        alcEventCallbackSOFT =
                reinterpret_cast<LPALCEVENTCALLBACKSOFT>(alcGetProcAddress(device_, "alcEventCallbackSOFT"));

        std::vector types{
                ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT,
                ALC_EVENT_TYPE_DEVICE_ADDED_SOFT,
                ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT};
        for (auto it = types.begin(); it != types.end(); ++it)
            if (alcEventIsSupportedSOFT(*it, ALC_PLAYBACK_DEVICE_SOFT) != ALC_EVENT_SUPPORTED_SOFT)
                it = types.erase(it);

        alcEventControlSOFT(types.size(), &types[0], AL_TRUE);
        alcEventCallbackSOFT(alc_event_callback_, static_cast<void *>(&callbacks_));
    } else {
        MIZU_LOG_WARN("ALC_SOFT_system_events not supported");
    }

    if (alcIsExtensionPresent(device_, "ALC_SOFT_reopen_device")) {
        alcReopenDeviceSOFT =
                reinterpret_cast<LPALCREOPENDEVICESOFT>(alcGetProcAddress(device_, "alcReopenDeviceSOFT"));
    } else {
        MIZU_LOG_WARN("ALC_SOFT_reopen_device not supported");
    }

    const ALCchar *name = device_specifier_(device_);
    MIZU_LOG_DEBUG("Opened \"{}\"", name);
}

AudioMgr::~AudioMgr() {
    unregister_callbacks_();

    for (const auto &[buffer, sources]: sources_) {
        for (const auto &source: sources) {
            alSourceStop(source);
            check_al_errors_();
        }

        if (!sources.empty()) {
            alDeleteSources(sources.size(), &sources[0]);
            check_al_errors_();
        }

        alDeleteBuffers(1, &buffer);
        check_al_errors_();
    }

    if (!alcMakeContextCurrent(nullptr) || !check_alc_errors_(device_))
        MIZU_LOG_ERROR("Failed to clear OpenAL context!");

    alcDestroyContext(ctx_);
    if (!check_alc_errors_(device_))
        MIZU_LOG_ERROR("Failed to destroy OpenAL context!");

    if (!alcCloseDevice(device_) || !check_alc_errors_(device_))
        MIZU_LOG_ERROR("Failed to close OpenAL device!");

    MIZU_LOG_DEBUG("Shutdown OpenAL");
}

Sound AudioMgr::load_sound(const std::filesystem::path &path) {
    auto sound = load_sound_impl(path);
    if (sound.buffer != 0) {
        sources_[sound.buffer] = {};

        sf_count_t frames = sound.sfinfo.frames;
        sf_count_t h = frames / (3600 * sound.sfinfo.samplerate);
        frames -= h * (3600 * sound.sfinfo.samplerate);
        sf_count_t m = frames / (60 * sound.sfinfo.samplerate);
        frames -= m * (60 * sound.sfinfo.samplerate);
        sf_count_t s = frames / sound.sfinfo.samplerate;
        frames -= s * sound.sfinfo.samplerate;
        auto ms = static_cast<sf_count_t>(1000 * (static_cast<double>(frames) / sound.sfinfo.samplerate));

        MIZU_LOG_DEBUG(
                "Loaded: {} ({}, {}hz, {})",
                sound.path,
                FormatName(sound.format),
                sound.sfinfo.samplerate,
                fmt::format("{}{}{:02}:{:02}.{:03}", h == 0 ? "" : fmt::to_string(h), h == 0 ? "" : ":", m, s, ms));
    }
    return sound;
}

void AudioMgr::unload_sound(Sound &sound) {}

void AudioMgr::play_sound(const Sound &sound, const PlayOptions &options) {
    ALuint source;
    alGenSources(1, &source);
    check_al_errors_();
    sources_[sound.buffer].emplace_back(source);

    alSourcef(sources_[sound.buffer].back(), AL_PITCH, options.pitch);
    alSourcef(sources_[sound.buffer].back(), AL_GAIN, options.volume);
    alSource3f(sources_[sound.buffer].back(), AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(sources_[sound.buffer].back(), AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(sources_[sound.buffer].back(), AL_LOOPING, options.looping ? AL_TRUE : AL_FALSE);
    alSourcei(sources_[sound.buffer].back(), AL_BUFFER, sound.buffer);

    alSourcePlay(sources_[sound.buffer].back());
    check_al_errors_();
}

const ALCchar *AudioMgr::device_specifier_(ALCdevice *device) {
    const ALCchar *name{};
    if (device) {
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || !check_alc_errors_(device)) {
            name = alcGetString(device, ALC_DEVICE_SPECIFIER);
            check_alc_errors_(device);
        }
    } else {
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(device, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);
        if (!name || !check_alc_errors_(device)) {
            name = alcGetString(device, ALC_DEFAULT_DEVICE_SPECIFIER);
            check_alc_errors_(device);
        }
    }
    return name;
}

void AudioMgr::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PUpdate>(callback_id_, [&](const auto &p) { update_(p.dt); });
    callbacks_.sub<PDefaultDeviceChanged>(callback_id_, [&](const auto &p) { default_device_changed_(p.device); });
    callbacks_.sub<PDeviceRemoved>(callback_id_, [&](const auto &p) { device_removed_(p.device); });
}

void AudioMgr::unregister_callbacks_() {
    callbacks_.unsub<PDeviceRemoved>(callback_id_);
    callbacks_.unsub<PDefaultDeviceChanged>(callback_id_);
    callbacks_.unsub<PUpdate>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

void AudioMgr::update_(double) {
    callbacks_.poll<PDefaultDeviceChanged>(callback_id_);

    for (auto s_it = sources_.begin(); s_it != sources_.end();) {
        auto &l = s_it->second;

        for (auto it = l.begin(); it != l.end();) {
            ALint state;
            alGetSourcei(*it, AL_SOURCE_STATE, &state);
            check_al_errors_();

            if (state == AL_STOPPED) {
                alDeleteSources(1, &(*it));
                check_al_errors_();
                it = l.erase(it);
                continue;
            }
            ++it;
        }

        ++s_it;
    }
}

void AudioMgr::default_device_changed_(ALCdevice *device) {
    if (alcReopenDeviceSOFT) {
        alcReopenDeviceSOFT(device_, nullptr, nullptr);
        check_alc_errors_(device_);
    }
}

void AudioMgr::device_removed_(ALCdevice *device) {
    // FIXME: Can't find a way to determine if the disconnected device is the current
    //        device, so we always reopen. This leads to an extremely tiny glitch in the
    //        sound when the device hasn't actually changed.
    if (alcReopenDeviceSOFT) {
        alcReopenDeviceSOFT(device_, nullptr, nullptr);
        check_alc_errors_(device_);
    }
}

bool check_al_errors_() {
    if (ALenum error = alGetError(); error != AL_NO_ERROR) {
        MIZU_LOG_ERROR(std::invoke([error] {
            switch (error) {
            case AL_INVALID_NAME: return "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
            case AL_INVALID_ENUM: return "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
            case AL_INVALID_VALUE: return "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION: the requested operation is not valid";
            case AL_OUT_OF_MEMORY:
                return "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            default: return fmt::format("UNKNOWN AL ERROR: {}", error).c_str();
            }
        }));
        return false;
    }
    return true;
}

bool check_alc_errors_(ALCdevice *device) {
    if (ALCenum error = alcGetError(device); error != ALC_NO_ERROR) {
        MIZU_LOG_ERROR(std::invoke([error] {
            switch (error) {
            case ALC_INVALID_VALUE: return "ALC_INVALID_VALUE: an invalid value was passed to an OpenAL function";
            case ALC_INVALID_DEVICE: return "ALC_INVALID_DEVICE: a bad device was passed to an OpenAL function";
            case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT: a bad context was passed to an OpenAL function";
            case ALC_INVALID_ENUM: return "ALC_INVALID_ENUM: an unknown enum value was passed to an OpenAL function";
            case ALC_OUT_OF_MEMORY:
                return "ALC_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
            default: return fmt::format("UNKNOWN ALC ERROR: {}", error).c_str();
            }
        }));
        return false;
    }
    return true;
}

void al_event_callback_(
        ALenum eventType,
        ALuint object,
        ALuint param,
        ALsizei length,
        const ALchar *message,
        void *userParam) noexcept {
#define STRINGIFY(e)                                                                                                   \
    case e: return #e;
    std::string eventType_string = std::invoke([eventType] {
        switch (eventType) {
            STRINGIFY(AL_EVENT_TYPE_BUFFER_COMPLETED_SOFT)
            STRINGIFY(AL_EVENT_TYPE_SOURCE_STATE_CHANGED_SOFT)
            STRINGIFY(AL_EVENT_TYPE_DISCONNECTED_SOFT)
        default: return "?";
        }
    });
#undef STRINGIFY
    MIZU_LOG_DEBUG("OpenAL: type={} msg={}", eventType_string, message);
}

void alc_event_callback_(
        ALCenum eventType,
        ALCenum deviceType,
        ALCdevice *device,
        ALCsizei length,
        const ALCchar *message,
        void *userParam) noexcept {
#define STRINGIFY(e)                                                                                                   \
    case e: return #e;
    std::string eventType_string = std::invoke([eventType] {
        switch (eventType) {
            STRINGIFY(ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT)
            STRINGIFY(ALC_EVENT_TYPE_DEVICE_ADDED_SOFT)
            STRINGIFY(ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT)
        default: return "?";
        }
    });
    std::string deviceType_string = std::invoke([deviceType] {
        switch (deviceType) {
            STRINGIFY(ALC_PLAYBACK_DEVICE_SOFT)
            STRINGIFY(ALC_CAPTURE_DEVICE_SOFT)
        default: return "?";
        }
    });
#undef STRINGIFY
    MIZU_LOG_DEBUG("OpenAL: type={} device_type={} msg={}", eventType_string, deviceType_string, message);

    auto *callbacks = static_cast<CallbackMgr *>(userParam);
    switch (eventType) {
    case ALC_EVENT_TYPE_DEFAULT_DEVICE_CHANGED_SOFT:
        callbacks->pub<PDefaultDeviceChanged>(eventType, deviceType, device, std::string(message, length));
        break;
    case ALC_EVENT_TYPE_DEVICE_ADDED_SOFT:
        // if (deviceType == ALC_PLAYBACK_DEVICE_SOFT)
        //     callbacks->pub<PDeviceAdded>(eventType, deviceType, device, std::string(message, length));
        break;
    case ALC_EVENT_TYPE_DEVICE_REMOVED_SOFT:
        if (deviceType == ALC_PLAYBACK_DEVICE_SOFT)
            callbacks->pub_nowait<PDeviceRemoved>(eventType, deviceType, device, std::string(message, length));
        break;
    default: MIZU_LOG_WARN("Unhandled event type: {}", eventType_string); break;
    }
}

enum FormatType { Int16, Float, IMA4, MSADPCM };

const char *FormatName(ALenum format) {
    switch (format) {
    case AL_FORMAT_MONO8: return "Mono, U8";
    case AL_FORMAT_MONO16: return "Mono, S16";
    case AL_FORMAT_MONO_FLOAT32: return "Mono, Float32";
    case AL_FORMAT_MONO_MULAW: return "Mono, muLaw";
    case AL_FORMAT_MONO_ALAW_EXT: return "Mono, aLaw";
    case AL_FORMAT_MONO_IMA4: return "Mono, IMA4 ADPCM";
    case AL_FORMAT_MONO_MSADPCM_SOFT: return "Mono, MS ADPCM";
    case AL_FORMAT_STEREO8: return "Stereo, U8";
    case AL_FORMAT_STEREO16: return "Stereo, S16";
    case AL_FORMAT_STEREO_FLOAT32: return "Stereo, Float32";
    case AL_FORMAT_STEREO_MULAW: return "Stereo, muLaw";
    case AL_FORMAT_STEREO_ALAW_EXT: return "Stereo, aLaw";
    case AL_FORMAT_STEREO_IMA4: return "Stereo, IMA4 ADPCM";
    case AL_FORMAT_STEREO_MSADPCM_SOFT: return "Stereo, MS ADPCM";
    case AL_FORMAT_QUAD8: return "Quadraphonic, U8";
    case AL_FORMAT_QUAD16: return "Quadraphonic, S16";
    case AL_FORMAT_QUAD32: return "Quadraphonic, Float32";
    case AL_FORMAT_QUAD_MULAW: return "Quadraphonic, muLaw";
    case AL_FORMAT_51CHN8: return "5.1 Surround, U8";
    case AL_FORMAT_51CHN16: return "5.1 Surround, S16";
    case AL_FORMAT_51CHN32: return "5.1 Surround, Float32";
    case AL_FORMAT_51CHN_MULAW: return "5.1 Surround, muLaw";
    case AL_FORMAT_61CHN8: return "6.1 Surround, U8";
    case AL_FORMAT_61CHN16: return "6.1 Surround, S16";
    case AL_FORMAT_61CHN32: return "6.1 Surround, Float32";
    case AL_FORMAT_61CHN_MULAW: return "6.1 Surround, muLaw";
    case AL_FORMAT_71CHN8: return "7.1 Surround, U8";
    case AL_FORMAT_71CHN16: return "7.1 Surround, S16";
    case AL_FORMAT_71CHN32: return "7.1 Surround, Float32";
    case AL_FORMAT_71CHN_MULAW: return "7.1 Surround, muLaw";
    case AL_FORMAT_BFORMAT2D_8: return "B-Format 2D, U8";
    case AL_FORMAT_BFORMAT2D_16: return "B-Format 2D, S16";
    case AL_FORMAT_BFORMAT2D_FLOAT32: return "B-Format 2D, Float32";
    case AL_FORMAT_BFORMAT2D_MULAW: return "B-Format 2D, muLaw";
    case AL_FORMAT_BFORMAT3D_8: return "B-Format 3D, U8";
    case AL_FORMAT_BFORMAT3D_16: return "B-Format 3D, S16";
    case AL_FORMAT_BFORMAT3D_FLOAT32: return "B-Format 3D, Float32";
    case AL_FORMAT_BFORMAT3D_MULAW: return "B-Format 3D, muLaw";
    case AL_FORMAT_UHJ2CHN8_SOFT: return "UHJ 2-channel, U8";
    case AL_FORMAT_UHJ2CHN16_SOFT: return "UHJ 2-channel, S16";
    case AL_FORMAT_UHJ2CHN_FLOAT32_SOFT: return "UHJ 2-channel, Float32";
    case AL_FORMAT_UHJ3CHN8_SOFT: return "UHJ 3-channel, U8";
    case AL_FORMAT_UHJ3CHN16_SOFT: return "UHJ 3-channel, S16";
    case AL_FORMAT_UHJ3CHN_FLOAT32_SOFT: return "UHJ 3-channel, Float32";
    case AL_FORMAT_UHJ4CHN8_SOFT: return "UHJ 4-channel, U8";
    case AL_FORMAT_UHJ4CHN16_SOFT: return "UHJ 4-channel, S16";
    case AL_FORMAT_UHJ4CHN_FLOAT32_SOFT: return "UHJ 4-channel, Float32";
    default: return "Unknown Format";
    }
}

Sound load_sound_impl(const std::filesystem::path &path) {
    SF_INFO sfinfo;
#if defined(MIZU_PLATFORM_WINDOWS)
    SNDFILE *sndfile = sf_wchar_open(path.c_str(), SFM_READ, &sfinfo);
#else
    SNDFILE *sndfile = sf_open(path.c_str(), SFM_READ, &sfinfo);
#endif
    if (!sndfile) {
        MIZU_LOG_ERROR("Failed to open audio file '{}': {}", path, sf_strerror(sndfile));
        return {};
    }
    if (sfinfo.frames < 1) {
        MIZU_LOG_ERROR("Bad sample count ({}) in audio file: '{}'", sfinfo.frames, path);
        return {};
    }

    FormatType sample_format = Int16;
    /* Detect a suitable format to load. Formats like Vorbis and Opus use float
     * natively, so load as float to avoid clipping when possible. Formats
     * larger than 16-bit can also use float to preserve a bit more precision.
     */
    switch (sfinfo.format & SF_FORMAT_SUBMASK) {
    case SF_FORMAT_PCM_24:
    case SF_FORMAT_PCM_32:
    case SF_FORMAT_FLOAT:
    case SF_FORMAT_DOUBLE:
    case SF_FORMAT_VORBIS:
    case SF_FORMAT_OPUS:
    case SF_FORMAT_ALAC_20:
    case SF_FORMAT_ALAC_24:
    case SF_FORMAT_ALAC_32:
    case 0x0080 /*SF_FORMAT_MPEG_LAYER_I*/:
    case 0x0081 /*SF_FORMAT_MPEG_LAYER_II*/:
    case 0x0082 /*SF_FORMAT_MPEG_LAYER_III*/:
        if (alIsExtensionPresent("AL_EXT_FLOAT32"))
            sample_format = Float;
        break;
    case SF_FORMAT_IMA_ADPCM:
        /* ADPCM formats require setting a block alignment as specified in the
         * file, which needs to be read from the wave 'fmt ' chunk manually
         * since libsndfile doesn't provide it in a format-agnostic way.
         */
        if (sfinfo.channels <= 2 && (sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV &&
            alIsExtensionPresent("AL_EXT_IMA4") && alIsExtensionPresent("AL_SOFT_block_alignment")) {
            sample_format = IMA4;
        }
        break;
    case SF_FORMAT_MS_ADPCM:
        if (sfinfo.channels <= 2 && (sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV &&
            alIsExtensionPresent("AL_SOFT_MSADPCM") && alIsExtensionPresent("AL_SOFT_block_alignment")) {
            sample_format = MSADPCM;
        }
        break;
    }

    ALint byteblockalign = 0;
    ALint splblockalign = 0;

    if (sample_format == IMA4 || sample_format == MSADPCM) {
        /* For ADPCM, lookup the wave file's "fmt " chunk, which is a
         * WAVEFORMATEX-based structure for the audio format.
         */
        SF_CHUNK_INFO inf = {"fmt ", 4, 0, nullptr};
        SF_CHUNK_ITERATOR *iter = sf_get_chunk_iterator(sndfile, &inf);

        /* If there's an issue getting the chunk or block alignment, load as
         * 16-bit and have libsndfile do the conversion.
         */
        if (!iter || sf_get_chunk_size(iter, &inf) != SF_ERR_NO_ERROR || inf.datalen < 14) {
            sample_format = Int16;
        } else {
            auto *fmtbuf = static_cast<ALubyte *>(calloc(inf.datalen, 1));
            inf.data = fmtbuf;
            if (sf_get_chunk_data(iter, &inf) != SF_ERR_NO_ERROR) {
                sample_format = Int16;
            } else {
                /* Read the nBlockAlign field, and convert from bytes- to
                 * samples-per-block (verifying it's valid by converting back
                 * and comparing to the original value).
                 */
                byteblockalign = fmtbuf[12] | (fmtbuf[13] << 8);
                if (sample_format == IMA4) {
                    splblockalign = (byteblockalign / sfinfo.channels - 4) / 4 * 8 + 1;
                    if (splblockalign < 1 || ((splblockalign - 1) / 2 + 4) * sfinfo.channels != byteblockalign)
                        sample_format = Int16;
                } else {
                    splblockalign = (byteblockalign / sfinfo.channels - 7) * 2 + 2;
                    if (splblockalign < 2 || ((splblockalign - 2) / 2 + 7) * sfinfo.channels != byteblockalign)
                        sample_format = Int16;
                }
            }
            free(fmtbuf);
        }
    }

    if (sample_format == Int16) {
        splblockalign = 1;
        byteblockalign = sfinfo.channels * 2;
    } else if (sample_format == Float) {
        splblockalign = 1;
        byteblockalign = sfinfo.channels * 4;
    }

    /* Figure out the OpenAL format from the file and desired sample type. */
    ALenum format = AL_NONE;
    if (sfinfo.channels == 1) {
        switch (sample_format) {
        case Int16: format = AL_FORMAT_MONO16; break;
        case Float: format = AL_FORMAT_MONO_FLOAT32; break;
        case IMA4: format = AL_FORMAT_MONO_IMA4; break;
        case MSADPCM: format = AL_FORMAT_MONO_MSADPCM_SOFT; break;
        }
    } else if (sfinfo.channels == 2) {
        switch (sample_format) {
        case Int16: format = AL_FORMAT_STEREO16; break;
        case Float: format = AL_FORMAT_STEREO_FLOAT32; break;
        case IMA4: format = AL_FORMAT_STEREO_IMA4; break;
        case MSADPCM: format = AL_FORMAT_STEREO_MSADPCM_SOFT; break;
        }
    } else if (sfinfo.channels == 3) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT) {
            switch (sample_format) {
            case Int16: format = AL_FORMAT_BFORMAT2D_16; break;
            case Float: format = AL_FORMAT_BFORMAT2D_FLOAT32; break;
            default:; // no change
            }
        }
    } else if (sfinfo.channels == 4) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0) == SF_AMBISONIC_B_FORMAT) {
            switch (sample_format) {
            case Int16: format = AL_FORMAT_BFORMAT3D_16; break;
            case Float: format = AL_FORMAT_BFORMAT3D_FLOAT32; break;
            default:; // no change
            }
        }
    }

    if (!format) {
        MIZU_LOG_ERROR("Unsupported channel count ({}) in audio file: '{}'", sfinfo.channels, path);
        sf_close(sndfile);
        return {};
    }

    if (sfinfo.frames / splblockalign > static_cast<sf_count_t>(INT_MAX / byteblockalign)) {
        MIZU_LOG_ERROR("Too many samples ({}) in audio file: '{}'", sfinfo.frames, path);
        sf_close(sndfile);
        return {};
    }

    /* Decode the whole audio file to a buffer. */
    void *membuf = malloc(static_cast<size_t>(sfinfo.frames / splblockalign * byteblockalign));
    sf_count_t num_frames;

    if (sample_format == Int16) {
        num_frames = sf_readf_short(sndfile, static_cast<short *>(membuf), sfinfo.frames);
    } else if (sample_format == Float) {
        num_frames = sf_readf_float(sndfile, static_cast<float *>(membuf), sfinfo.frames);
    } else {
        sf_count_t count = sfinfo.frames / splblockalign * byteblockalign;
        num_frames = sf_read_raw(sndfile, membuf, count);
        if (num_frames > 0)
            num_frames = num_frames / byteblockalign * splblockalign;
    }
    if (num_frames < 1) {
        free(membuf);
        sf_close(sndfile);
        MIZU_LOG_ERROR("Failed to read samples ({}) in audio file: '{}'", num_frames, path);
        return {};
    }
    const auto num_bytes = static_cast<ALsizei>(num_frames / splblockalign * byteblockalign);

    /* Buffer the audio data into a new buffer object, then free the data and
     * close the file.
     */
    ALuint buffer = 0;
    alGenBuffers(1, &buffer);
    if (splblockalign > 1)
        alBufferi(buffer, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splblockalign);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
    free(membuf);

    if (sf_close(sndfile) != 0)
        MIZU_LOG_ERROR("Failed to close audio file '{}': {}", path, sf_strerror(sndfile));

    /* Check if an error occurred, and clean up if so. */
    if (!check_al_errors_()) {
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return {};
    }

    return {buffer, path, format, sfinfo};
}
} // namespace mizu
