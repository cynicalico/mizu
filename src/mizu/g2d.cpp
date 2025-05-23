#include "mizu/g2d.hpp"
#include <cmath>
#include "mizu/payloads.hpp"

namespace mizu {
G2d::G2d(gloo::GlContext &gl, CallbackMgr &callbacks) : gl_(gl), callbacks_(callbacks) {
    register_callbacks_();
}

G2d::~G2d() {
    unregister_callbacks_();
}

void G2d::set_clear_color(const Color &c) {
    clear_color_ = c.gl_color();
}

Rgba G2d::clear_color() const {
    return rgba(
            static_cast<std::uint8_t>(std::round(clear_color_.r * 255.0)),
            static_cast<std::uint8_t>(std::round(clear_color_.g * 255.0)),
            static_cast<std::uint8_t>(std::round(clear_color_.b * 255.0)),
            static_cast<std::uint8_t>(std::round(clear_color_.a * 255.0))
    );
}

gloo::ShaderBuilder G2d::shader_builder() const {
    return gloo::ShaderBuilder(gl_.ctx);
}

void G2d::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPostDraw>(callback_id_, [&](const auto &) { draw_(); });
}

void G2d::unregister_callbacks_() {
    callbacks_.unsub<PPostDraw>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

void G2d::draw_() {
    gl_.ctx.ClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
    gl_.ctx.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
} // namespace mizu
