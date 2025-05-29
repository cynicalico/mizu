#include "mizu/g2d.hpp"

namespace mizu {
G2d::G2d(gloo::GlContext &gl, CallbackMgr &callbacks) : gl_(gl), callbacks_(callbacks) {
    register_callbacks_();
}

G2d::~G2d() {
    unregister_callbacks_();
}

void G2d::clear(const Color &color, ClearBit clear_bits) {
    auto gl_color = color.gl_color();
    gl_.ctx.ClearColor(gl_color.r, gl_color.g, gl_color.b, gl_color.a);
    gl_.ctx.Clear(unwrap(clear_bits));
}

gloo::ShaderBuilder G2d::shader_builder() const {
    return gloo::ShaderBuilder(gl_.ctx);
}

gloo::VertexArrayBuilder G2d::vertex_array_builder() const {
    return gloo::VertexArrayBuilder(gl_.ctx);
}

void G2d::register_callbacks_() {
    callback_id_ = callbacks_.reg();
}

void G2d::unregister_callbacks_() {
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}
} // namespace mizu
