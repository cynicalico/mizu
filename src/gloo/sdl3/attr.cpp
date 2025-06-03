#include "gloo/sdl3/attr.hpp"
#include "mizu/log.hpp"

namespace gloo::sdl3 {
std::optional<int> Attr::red_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get red (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::green_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get green (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::blue_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get blue (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::alpha_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get alpha (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::buffer_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get buffer (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<bool> Attr::double_buffer() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &v)) {
        MIZU_LOG_ERROR("Failed to get double buffer state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<int> Attr::depth_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get depth (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::stencil_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get stencil (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::accum_red_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_RED_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get accum red (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::accum_green_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_GREEN_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get accum green (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::accum_blue_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_BLUE_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get accum blue (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::accum_alpha_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, &v)) {
        MIZU_LOG_ERROR("Failed to get accum alpha (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<bool> Attr::stereo_3d() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_STEREO, &v)) {
        MIZU_LOG_ERROR("Failed to get stereo 3D state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<int> Attr::multisample_buffers() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &v)) {
        MIZU_LOG_ERROR("Failed to get multisample buffer count: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> Attr::multisample_samples() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &v)) {
        MIZU_LOG_ERROR("Failed to get multisample sample count: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<bool> Attr::accelerated() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &v)) {
        MIZU_LOG_ERROR("Failed to get accelerated visual state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<ContextVersion> Attr::context_version() {
    auto version = ContextVersion(0, 0);
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &version.major)) {
        MIZU_LOG_ERROR("Failed to get context major version: {}", SDL_GetError());
        return std::nullopt;
    }
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &version.minor)) {
        MIZU_LOG_ERROR("Failed to get context minor version: {}", SDL_GetError());
        return std::nullopt;
    }
    return version;
}

std::optional<ContextFlags> Attr::context_flags() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &v)) {
        MIZU_LOG_ERROR("Failed to get context flags: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<ContextFlags>(v);
}

std::optional<Profile> Attr::profile() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &v)) {
        MIZU_LOG_ERROR("Failed to get context profile: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<Profile>(v);
}

std::optional<bool> Attr::share_with_current_context() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, &v)) {
        MIZU_LOG_ERROR("Failed to get share with current context: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<bool> Attr::framebuffer_srgb_capable() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, &v)) {
        MIZU_LOG_ERROR("Failed to get framebuffer srgb capable: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<ContextRelease> Attr::context_release_behavior() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, &v)) {
        MIZU_LOG_ERROR("Failed to get context release behavior: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<ContextRelease>(v);
}

std::optional<ContextResetNotification> Attr::context_reset_notification() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, &v)) {
        MIZU_LOG_ERROR("Failed to get context reset notification: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<ContextResetNotification>(v);
}

std::optional<bool> Attr::error_checking_disabled() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_NO_ERROR, &v)) {
        MIZU_LOG_ERROR("Failed to get context error checking state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

void Attr::set_red_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_RED_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set red bit size: {}", SDL_GetError());
}

void Attr::set_green_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set green bit size: {}", SDL_GetError());
}

void Attr::set_blue_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set blue bit size: {}", SDL_GetError());
}

void Attr::set_alpha_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set alpha bit size: {}", SDL_GetError());
}

void Attr::set_buffer_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set buffer bit size: {}", SDL_GetError());
}

void Attr::set_double_buffer(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, v ? 1 : 0))
        MIZU_LOG_ERROR("Failed to set double buffer state: {}", SDL_GetError());
}

void Attr::set_depth_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set depth bit size: {}", SDL_GetError());
}

void Attr::set_stencil_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set stencil bit size: {}", SDL_GetError());
}

void Attr::set_accum_red_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set accum red bit size: {}", SDL_GetError());
}

void Attr::set_accum_green_size(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set accum green bit size: {}", SDL_GetError());
}

void Attr::set_accum_blue_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set accum blue bit size: {}", SDL_GetError());
}

void Attr::set_accum_alpha_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, min_size))
        MIZU_LOG_ERROR("Failed to set accum alpha bit size: {}", SDL_GetError());
}

void Attr::set_stereo_3d(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_STEREO, v ? 1 : 0))
        MIZU_LOG_ERROR("Failed to set stereo 3D: {}", SDL_GetError());
}

void Attr::set_multisample_buffers(int buffer_count) {
    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, buffer_count))
        MIZU_LOG_ERROR("Failed to set multisample buffer count: {}", SDL_GetError());
}

void Attr::set_multisample_samples(int sample_count) {
    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, sample_count))
        MIZU_LOG_ERROR("Failed to set multisample sample count: {}", SDL_GetError());
}

void Attr::set_accelerated(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set accelerated visual state: {}", SDL_GetError());
}

void Attr::set_context_version(ContextVersion version) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version.major))
        SPDLOG_ERROR("Failed to set context major version: {}", SDL_GetError());
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version.minor))
        SPDLOG_ERROR("Failed to set context minor version: {}", SDL_GetError());
}

ContextFlagsBuilder Attr::set_context_flags() {
    return ContextFlagsBuilder();
}

void Attr::set_context_profile(Profile profile) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, static_cast<int>(profile)))
        SPDLOG_ERROR("Failed to set context profile: {}", SDL_GetError());
}

void Attr::set_share_with_current_context(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set share with current context: {}", SDL_GetError());
}

void Attr::set_framebuffer_srgb_capable(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set framebuffer srgb capable: {}", SDL_GetError());
}

void Attr::set_context_release_behavior(ContextRelease behavior) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, static_cast<int>(behavior)))
        SPDLOG_ERROR("Failed to set context release behavior: {}", SDL_GetError());
}

void Attr::set_context_reset_notification(ContextResetNotification notif) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, static_cast<int>(notif)))
        SPDLOG_ERROR("Failed to set context reset notification: {}", SDL_GetError());
}

void Attr::set_error_checking_disabled(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set context error checking state: {}", SDL_GetError());
}
} // namespace gloo::sdl3
