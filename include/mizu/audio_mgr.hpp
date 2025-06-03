#ifndef MIZU_AUDIO_MGR_HPP
#define MIZU_AUDIO_MGR_HPP

#include <al.h>
#include <alc.h>
#include <alext.h>
#include <filesystem>
#include <sndfile.hh>
#include "mizu/callback_mgr.hpp"
#include "mizu/class_helpers.hpp"

namespace mizu {
inline LPALEVENTCONTROLSOFT alEventControlSOFT;
inline LPALEVENTCALLBACKSOFT alEventCallbackSOFT;
inline LPALCEVENTISSUPPORTEDSOFT alcEventIsSupportedSOFT;
inline LPALCEVENTCONTROLSOFT alcEventControlSOFT;
inline LPALCEVENTCALLBACKSOFT alcEventCallbackSOFT;
inline LPALCREOPENDEVICESOFT alcReopenDeviceSOFT;

struct Sound {
    ALuint buffer{0}; // 0 if empty
    std::filesystem::path path{};
    ALenum format{AL_NONE};
    SF_INFO sfinfo{};
};

struct PlayOptions {
    float pitch{1.0f}; // valid range: 0.5 - 2.0
    float volume{1.0f}; // valid range: 0.0 - 1.0
    bool looping{false};
};

class AudioMgr {
public:
    explicit AudioMgr(CallbackMgr &callbacks);
    ~AudioMgr();

    NO_COPY(AudioMgr)
    NO_MOVE(AudioMgr)

    Sound load_sound(const std::filesystem::path &path);
    void unload_sound(Sound &sound);

    void play_sound(const Sound &sound, const PlayOptions &options = {});

private:
    std::string device_specifier_{};
    ALCdevice *device_{nullptr};
    ALCcontext *ctx_{nullptr};

    std::unordered_map<ALuint, std::vector<ALuint>> sources_{};

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();

    void update_(double dt);
    void default_device_changed_(ALCdevice *device);
};
} // namespace mizu

#endif // MIZU_AUDIO_MGR_HPP
