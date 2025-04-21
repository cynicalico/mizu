#include "gloo/sdl3/gl_attr.hpp"
#include "mizu/log.hpp"

namespace gloo::sdl3 {
std::optional<int> GlAttr::red_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get red (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::green_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get green (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::blue_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get blue (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::alpha_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get alpha (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::buffer_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get buffer (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<bool> GlAttr::double_buffer() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &v)) {
        SPDLOG_ERROR("Failed to get double buffer state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<int> GlAttr::depth_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get depth (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::stencil_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get stencil (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::accum_red_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_RED_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get accum red (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::accum_green_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_GREEN_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get accum green (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::accum_blue_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_BLUE_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get accum blue (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::accum_alpha_bits() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, &v)) {
        SPDLOG_ERROR("Failed to get accum alpha (size) bits: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<bool> GlAttr::stereo_3d() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_STEREO, &v)) {
        SPDLOG_ERROR("Failed to get stereo 3D state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<int> GlAttr::multisample_buffers() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &v)) {
        SPDLOG_ERROR("Failed to get multisample buffer count: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<int> GlAttr::multisample_samples() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &v)) {
        SPDLOG_ERROR("Failed to get multisample sample count: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<bool> GlAttr::accelerated() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &v)) {
        SPDLOG_ERROR("Failed to get accelerated visual state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v;
}

std::optional<GlContextVersion> GlAttr::context_version() {
    auto version = GlContextVersion(0, 0);
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &version.major)) {
        SPDLOG_ERROR("Failed to get context major version: {}", SDL_GetError());
        return std::nullopt;
    }
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &version.minor)) {
        SPDLOG_ERROR("Failed to get context minor version: {}", SDL_GetError());
        return std::nullopt;
    }
    return version;
}

std::optional<GlContextFlags> GlAttr::context_flags() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &v)) {
        SPDLOG_ERROR("Failed to get context flags: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<GlContextFlags>(v);
}

std::optional<GlProfile> GlAttr::profile() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &v)) {
        SPDLOG_ERROR("Failed to get context profile: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<GlProfile>(v);
}

std::optional<bool> GlAttr::share_with_current_context() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, &v)) {
        SPDLOG_ERROR("Failed to get share with current context: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<bool> GlAttr::framebuffer_srgb_capable() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, &v)) {
        SPDLOG_ERROR("Failed to get framebuffer srgb capable: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

std::optional<GlContextRelease> GlAttr::context_release_behavior() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, &v)) {
        SPDLOG_ERROR("Failed to get context release behavior: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<GlContextRelease>(v);
}

std::optional<GlContextResetNotification> GlAttr::context_reset_notification() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, &v)) {
        SPDLOG_ERROR("Failed to get context reset notification: {}", SDL_GetError());
        return std::nullopt;
    }
    return static_cast<GlContextResetNotification>(v);
}

std::optional<bool> GlAttr::error_checking_disabled() {
    int v;
    if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_NO_ERROR, &v)) {
        SPDLOG_ERROR("Failed to get context error checking state: {}", SDL_GetError());
        return std::nullopt;
    }
    return v == 1;
}

void GlAttr::set_red_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_RED_SIZE, min_size)) SPDLOG_ERROR("Failed to set red bit size: {}", SDL_GetError());
}

void GlAttr::set_green_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, min_size))
        SPDLOG_ERROR("Failed to set green bit size: {}", SDL_GetError());
}

void GlAttr::set_blue_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, min_size))
        SPDLOG_ERROR("Failed to set blue bit size: {}", SDL_GetError());
}

void GlAttr::set_alpha_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, min_size))
        SPDLOG_ERROR("Failed to set alpha bit size: {}", SDL_GetError());
}

void GlAttr::set_buffer_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, min_size))
        SPDLOG_ERROR("Failed to set buffer bit size: {}", SDL_GetError());
}

void GlAttr::set_double_buffer(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set double buffer state: {}", SDL_GetError());
}

void GlAttr::set_depth_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, min_size))
        SPDLOG_ERROR("Failed to set depth bit size: {}", SDL_GetError());
}

void GlAttr::set_stencil_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, min_size))
        SPDLOG_ERROR("Failed to set stencil bit size: {}", SDL_GetError());
}

void GlAttr::set_accum_red_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, min_size))
        SPDLOG_ERROR("Failed to set accum red bit size: {}", SDL_GetError());
}

void GlAttr::set_accum_green_size(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, min_size))
        SPDLOG_ERROR("Failed to set accum green bit size: {}", SDL_GetError());
}

void GlAttr::set_accum_blue_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, min_size))
        SPDLOG_ERROR("Failed to set accum blue bit size: {}", SDL_GetError());
}

void GlAttr::set_accum_alpha_bits(int min_size) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, min_size))
        SPDLOG_ERROR("Failed to set accum alpha bit size: {}", SDL_GetError());
}

void GlAttr::set_stereo_3d(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_STEREO, v ? 1 : 0)) SPDLOG_ERROR("Failed to set stereo 3D: {}", SDL_GetError());
}

void GlAttr::set_multisample_buffers(int buffer_count) {
    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, buffer_count))
        SPDLOG_ERROR("Failed to set multisample buffer count: {}", SDL_GetError());
}

void GlAttr::set_multisample_samples(int sample_count) {
    if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, sample_count))
        SPDLOG_ERROR("Failed to set multisample sample count: {}", SDL_GetError());
}

void GlAttr::set_accelerated(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set accelerated visual state: {}", SDL_GetError());
}

void GlAttr::set_context_version(GlContextVersion version) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version.major))
        SPDLOG_ERROR("Failed to set context major version: {}", SDL_GetError());
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version.minor))
        SPDLOG_ERROR("Failed to set context minor version: {}", SDL_GetError());
}

GlContextFlagsBuilder GlAttr::set_context_flags() {
    return GlContextFlagsBuilder();
}

void GlAttr::set_context_profile(GlProfile profile) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, static_cast<int>(profile)))
        SPDLOG_ERROR("Failed to set context profile: {}", SDL_GetError());
}

void GlAttr::set_share_with_current_context(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set share with current context: {}", SDL_GetError());
}

void GlAttr::set_framebuffer_srgb_capable(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set framebuffer srgb capable: {}", SDL_GetError());
}

void GlAttr::set_context_release_behavior(GlContextRelease behavior) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_RELEASE_BEHAVIOR, static_cast<int>(behavior)))
        SPDLOG_ERROR("Failed to set context release behavior: {}", SDL_GetError());
}

void GlAttr::set_context_reset_notification(GlContextResetNotification notif) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_RESET_NOTIFICATION, static_cast<int>(notif)))
        SPDLOG_ERROR("Failed to set context reset notification: {}", SDL_GetError());
}

void GlAttr::set_error_checking_disabled(bool v) {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_NO_ERROR, v ? 1 : 0))
        SPDLOG_ERROR("Failed to set context error checking state: {}", SDL_GetError());
}
} // namespace gloo::sdl3
