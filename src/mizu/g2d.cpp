#include "mizu/g2d.hpp"
#include <SDL3/SDL_video.h>
#include "mizu/payloads.hpp"

namespace mizu {
G2d::G2d(CallbackMgr &callbacks, gloo::Context &gl, Window *window)
    : gl_(gl), window_(window), batcher_(gl_), callbacks_(callbacks) {
    register_callbacks_();
}

G2d::~G2d() {
    unregister_callbacks_();
}

bool G2d::vsync() const {
    int vsync;
    SDL_GL_GetSwapInterval(&vsync);

    return vsync == 1;
}

void G2d::set_vsync(bool enabled) {
    SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}

void G2d::clear(const Color &color, ClearBit clear_bits) {
    auto gl_color = color.gl_color();
    gl_.ctx.ClearColor(gl_color.r, gl_color.g, gl_color.b, gl_color.a);
    gl_.ctx.Clear(unwrap(clear_bits));
}

void G2d::point(glm::vec2 p, const Color &color) {
    auto gl_color = color.gl_color();
    batcher_.add(BatchType::Points, {p.x, p.y, 0.0, gl_color.r, gl_color.g, gl_color.b});
}

void G2d::line(glm::vec2 p0, glm::vec2 p1, glm::vec3 rot, const Color &color) {
    auto gl_color = color.gl_color();
    // clang-format off
    batcher_.add(BatchType::Lines, {
        p0.x, p0.y, 0.0, gl_color.r, gl_color.g, gl_color.b, rot.x, rot.y, glm::radians(rot.z),
        p1.x, p1.y, 0.0, gl_color.r, gl_color.g, gl_color.b, rot.x, rot.y, glm::radians(rot.z),
    });
    // clang-format on
}

void G2d::line(glm::vec2 p0, glm::vec2 p1, const Color &color) {
    line(p0, p1, glm::vec3(0.0), color);
}

void G2d::triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec3 rot, const Color &color) {
    auto gl_color = color.gl_color();
    // clang-format off
    batcher_.add(BatchType::Triangles, {
        p0.x, p0.y, 0.0, gl_color.r, gl_color.g, gl_color.b, rot.x, rot.y, glm::radians(rot.z),
        p1.x, p1.y, 0.0, gl_color.r, gl_color.g, gl_color.b, rot.x, rot.y, glm::radians(rot.z),
        p2.x, p2.y, 0.0, gl_color.r, gl_color.g, gl_color.b, rot.x, rot.y, glm::radians(rot.z),
    });
    // clang-format on
}

void G2d::triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color) {
    triangle(p0, p1, p2, glm::vec3(0.0), color);
}

void G2d::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPostDraw>(callback_id_, [&](const auto &) { post_draw_(); });
}

void G2d::unregister_callbacks_() {
    callbacks_.unsub<PPostDraw>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

void G2d::post_draw_() {
    batcher_.draw(window_->projection());
    batcher_.clear();
}
} // namespace mizu
