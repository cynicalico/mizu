#include "gloo/context.hpp"
#include "mizu/log.hpp"

namespace gloo {
std::optional<ContextVersion> Context::load(GLADloadfunc func) {
    auto glad_version = gladLoadGLContext(&ctx, func);
    if (glad_version == 0)
        return std::nullopt;
    return ContextVersion(GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));
}

void Context::enable(Capability cap) {
    ctx.Enable(static_cast<GLenum>(cap));
    CHECK_GL_ERROR(ctx, Enable);
}

void Context::disable(Capability cap) {
    ctx.Disable(static_cast<GLenum>(cap));
    CHECK_GL_ERROR(ctx, Disable);
}

bool Context::is_enabled(Capability cap) const {
    bool enabled = ctx.IsEnabled(static_cast<GLenum>(cap)) == GL_TRUE;
    CHECK_GL_ERROR(ctx, IsEnabled);
    return enabled;
}

void Context::clear_depth(float depth) {
    ctx.ClearDepth(depth);
    CHECK_GL_ERROR(ctx, ClearDepth);
}

void Context::blend_func(BlendFunc sfactor, BlendFunc dfactor) {
    ctx.BlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
    CHECK_GL_ERROR(ctx, BlendFunc);
}

void Context::depth_func(DepthFunc func) {
    ctx.DepthFunc(static_cast<GLenum>(func));
    CHECK_GL_ERROR(ctx, DepthFunc);
}

void Context::clip_control(ClipOrigin origin, ClipDepth depth) {
    ctx.ClipControl(static_cast<GLenum>(origin), static_cast<GLenum>(depth));
    CHECK_GL_ERROR(ctx, ClipControl);
}

void Context::depth_mask(bool enabled) {
    ctx.DepthMask(enabled ? GL_TRUE : GL_FALSE);
    CHECK_GL_ERROR(ctx, DepthMask);
}

void Context::debug_message_callback(GLDEBUGPROC callback, const void *user_param) {
    ctx.DebugMessageCallback(callback, user_param);
    CHECK_GL_ERROR(ctx, DebugMessageCallback);
}
} // namespace gloo
