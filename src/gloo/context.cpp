#include "gloo/context.hpp"

namespace gloo {
std::optional<ContextVersion> Context::load(GLADloadfunc func) {
    auto glad_version = gladLoadGLContext(&ctx, func);
    if (glad_version == 0)
        return std::nullopt;
    return ContextVersion(GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));
}

void Context::enable(Capability cap) {
    ctx.Enable(static_cast<GLenum>(cap));
}

void Context::disable(Capability cap) {
    ctx.Disable(static_cast<GLenum>(cap));
}

bool Context::is_enabled(Capability cap) const {
    return ctx.IsEnabled(static_cast<GLenum>(cap)) == GL_TRUE;
}

void Context::blend_func(BlendFunc sfactor, BlendFunc dfactor) {
    ctx.BlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
}
void Context::debug_message_callback(GLDEBUGPROC callback, const void *user_param) {
    ctx.DebugMessageCallback(callback, user_param);
}
} // namespace gloo
