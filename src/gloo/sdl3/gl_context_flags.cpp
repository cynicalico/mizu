#include "gloo/sdl3/gl_context_flags.hpp"
#include "mizu/log.hpp"

namespace gloo::sdl3 {
GlContextFlagsBuilder::GlContextFlagsBuilder() : flags_(GlContextFlags::None) {}

GlContextFlagsBuilder &GlContextFlagsBuilder::debug() {
    flags_ |= GlContextFlags::Debug;
    return *this;
}

GlContextFlagsBuilder &GlContextFlagsBuilder::forward_compatible() {
    flags_ |= GlContextFlags::ForwardCompatible;
    return *this;
}

GlContextFlagsBuilder &GlContextFlagsBuilder::robust_access() {
    flags_ |= GlContextFlags::RobustAccess;
    return *this;
}

GlContextFlagsBuilder &GlContextFlagsBuilder::reset_isolation() {
    flags_ |= GlContextFlags::ResetIsolation;
    return *this;
}

void GlContextFlagsBuilder::set() {
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, unwrap(flags_)))
        SPDLOG_ERROR("Failed to set context flags: {}", SDL_GetError());
}
} // namespace gloo::sdl3
