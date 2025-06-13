#ifndef GLOO_SDL3_GL_CONTEXT_FLAGS_HPP
#define GLOO_SDL3_GL_CONTEXT_FLAGS_HPP

#include <SDL3/SDL_video.h>
#include "mizu/util/enum_class_helpers.hpp"

namespace gloo::sdl3 {
enum class ContextFlags : int {
    None = 0,
    Debug = SDL_GL_CONTEXT_DEBUG_FLAG,
    ForwardCompatible = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG,
    RobustAccess = SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG,
    ResetIsolation = SDL_GL_CONTEXT_RESET_ISOLATION_FLAG
};

class ContextFlagsBuilder {
public:
    ContextFlagsBuilder();

    ContextFlagsBuilder &debug();
    ContextFlagsBuilder &forward_compatible();
    ContextFlagsBuilder &robust_access();
    ContextFlagsBuilder &reset_isolation();

    void set();

private:
    ContextFlags flags_;
};
} // namespace gloo::sdl3

ENUM_CLASS_ENABLE_BITOPS(gloo::sdl3::ContextFlags);

#endif // GLOO_SDL3_GL_CONTEXT_FLAGS_HPP
