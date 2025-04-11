#ifndef MIZU_GL_CONTEXT_HPP
#define MIZU_GL_CONTEXT_HPP
#include "enum_class_bitops.hpp"


#include <SDL3/SDL_video.h>
#include <optional>

namespace mizu {

struct GlContextVersion {
    int major;
    int minor;

    GlContextVersion(int major, int minor) : major(major), minor(minor) {}
};

enum class GlProfile {
    Core = SDL_GL_CONTEXT_PROFILE_CORE,
    Compatibility = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY,
    Es = SDL_GL_CONTEXT_PROFILE_ES,
};

enum class GlContextFlags {
    None = 0,
    Debug = SDL_GL_CONTEXT_DEBUG_FLAG,
    ForwardCompatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
    RobustAccess = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
    ResetIsolation = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
};

enum class GlContextRelease {
    None = SDL_GL_CONTEXT_RELEASE_BEHAVIOR_NONE,
    Flush = SDL_GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH,
};

enum class GlContextResetNotification {
    None = SDL_GL_CONTEXT_RESET_NO_NOTIFICATION,
    LoseContext = SDL_GL_CONTEXT_RESET_LOSE_CONTEXT,
};

class GlContextFlagsBuilder;

class GlAttr {
public:
    static std::optional<int> red_bits();
    static std::optional<int> green_bits();
    static std::optional<int> blue_bits();
    static std::optional<int> alpha_bits();
    static std::optional<int> buffer_bits();
    static std::optional<bool> double_buffer();
    static std::optional<int> depth_bits();
    static std::optional<int> stencil_bits();
    static std::optional<int> accum_red_bits();
    static std::optional<int> accum_green_bits();
    static std::optional<int> accum_blue_bits();
    static std::optional<int> accum_alpha_bits();
    static std::optional<bool> stereo_3d();
    static std::optional<int> multisample_buffers();
    static std::optional<int> multisample_samples();
    static std::optional<bool> accelerated();
    static std::optional<GlContextVersion> context_version();
    static std::optional<GlContextFlags> context_flags();
    static std::optional<GlProfile> profile();
    static std::optional<bool> share_with_current_context();
    static std::optional<bool> framebuffer_srgb_capable();
    static std::optional<GlContextRelease> context_release_behavior();
    static std::optional<GlContextResetNotification> context_reset_notification();
    static std::optional<bool> error_checking_disabled();

    static void set_red_bits(int min_size);
    static void set_green_bits(int min_size);
    static void set_blue_bits(int min_size);
    static void set_alpha_bits(int min_size);
    static void set_buffer_bits(int min_size);
    static void set_double_buffer(bool v);
    static void set_depth_bits(int min_size);
    static void set_stencil_bits(int min_size);
    static void set_accum_red_bits(int min_size);
    static void set_accum_green_size(int min_size);
    static void set_accum_blue_bits(int min_size);
    static void set_accum_alpha_bits(int min_size);

    static void set_stereo_3d(bool v);
    static void set_multisample_buffers(int buffer_count);
    static void set_multisample_samples(int sample_count);
    static void set_accelerated(bool v);
    static void set_context_version(GlContextVersion version);
    static GlContextFlagsBuilder set_context_flags();
    static void set_context_profile(GlProfile profile);
    static void set_share_with_current_context(bool v);
    static void set_framebuffer_srgb_capable(bool v);
    static void set_context_release_behavior(GlContextRelease behavior);
    static void set_context_reset_notification(GlContextResetNotification notif);
    static void set_error_checking_disabled(bool v);
};

class GlContextFlagsBuilder {
public:
    GlContextFlagsBuilder();

    GlContextFlagsBuilder &debug();
    GlContextFlagsBuilder &forward_compatible();
    GlContextFlagsBuilder &robust_access();
    GlContextFlagsBuilder &reset_isolation();

    void set();

private:
    GlContextFlags flags_;
};

} // namespace mizu

ENUM_CLASS_ENABLE_BITOPS(mizu::GlContextFlags);

#endif // MIZU_GL_CONTEXT_HPP
