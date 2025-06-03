#include "gloo/sdl3/context_flags.hpp"
#include "mizu/log.hpp"

namespace gloo::sdl3 {
ContextFlagsBuilder::ContextFlagsBuilder()
    : flags_(ContextFlags::None) {}

ContextFlagsBuilder &ContextFlagsBuilder::debug() {
    flags_ |= ContextFlags::Debug;
    return *this;
}

ContextFlagsBuilder &ContextFlagsBuilder::forward_compatible() {
    flags_ |= ContextFlags::ForwardCompatible;
    return *this;
}

ContextFlagsBuilder &ContextFlagsBuilder::robust_access() {
    flags_ |= ContextFlags::RobustAccess;
    return *this;
}

ContextFlagsBuilder &ContextFlagsBuilder::reset_isolation() {
    flags_ |= ContextFlags::ResetIsolation;
    return *this;
}

void ContextFlagsBuilder::set() {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, unwrap(flags_)))
        MIZU_LOG_ERROR("Failed to set context flags: {}", SDL_GetError());
}
} // namespace gloo::sdl3
