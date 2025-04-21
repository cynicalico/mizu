#ifndef GLOO_SDL3_GL_CONTEXT_FLAGS_HPP
#define GLOO_SDL3_GL_CONTEXT_FLAGS_HPP

#include <SDL3/SDL_video.h>
#include "mizu/enum_class_helpers.hpp"

namespace gloo::sdl3 {
enum class GlContextFlags : int {
    None = 0,
    Debug = SDL_GL_CONTEXT_DEBUG_FLAG,
    ForwardCompatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
    RobustAccess = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
    ResetIsolation = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
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
} // namespace gloo::sdl3

ENUM_CLASS_ENABLE_BITOPS(gloo::sdl3::GlContextFlags);

#endif // GLOO_SDL3_GL_CONTEXT_FLAGS_HPP
