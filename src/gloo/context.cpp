#include "gloo/context.hpp"

namespace gloo {
std::optional<GlContextVersion> GlContext::load(GLADloadfunc func) {
    auto glad_version = gladLoadGLContext(&ctx, func);
    if (glad_version == 0)
        return std::nullopt;
    return GlContextVersion(GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));
}

void GlContext::enable(GlCapability cap) { ctx.Enable(static_cast<GLenum>(cap)); }

void GlContext::disable(GlCapability cap) { ctx.Disable(static_cast<GLenum>(cap)); }

bool GlContext::is_enabled(GlCapability cap) const { return ctx.IsEnabled(static_cast<GLenum>(cap)) == GL_TRUE; }

void GlContext::blend_func(GlBlendFunc sfactor, GlBlendFunc dfactor) {
    ctx.BlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
}
} // namespace gloo
