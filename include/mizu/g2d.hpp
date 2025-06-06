#ifndef MIZU_G2D_HPP
#define MIZU_G2D_HPP

#include <glm/vec2.hpp>
#include "gloo/context.hpp"
#include "mizu/batcher.hpp"
#include "mizu/callback_mgr.hpp"
#include "mizu/class_helpers.hpp"
#include "mizu/color.hpp"
#include "mizu/enum_class_helpers.hpp"
#include "mizu/shapes.hpp"
#include "mizu/window.hpp"

namespace mizu {
enum class ClearBit : GLenum {
    Color = GL_COLOR_BUFFER_BIT,
    Depth = GL_DEPTH_BUFFER_BIT,
    Stencil = GL_STENCIL_BUFFER_BIT
};

class G2d {
public:
    G2d(CallbackMgr &callbacks, gloo::Context &gl, Window *window);

    ~G2d();

    NO_COPY(G2d)
    NO_MOVE(G2d)

    bool vsync() const;
    void set_vsync(bool enabled);

    void clear(const Color &color, ClearBit clear_bits);

    void point(glm::vec2 p, const Color &color);

    void line(glm::vec2 p0, glm::vec2 p1, glm::vec3 rot, const Color &color);
    void line(glm::vec2 p0, glm::vec2 p1, const Color &color);

    void triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec3 rot, const Color &color);
    void triangle(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void point(const Point<Color> &p);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void line(const Line<Color> &l);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void triangle(const Triangle<Color> &t);

private:
    gloo::Context &gl_;
    Window *window_;

    Batcher batcher_;

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();

    void post_draw_();
};

template<typename Color>
    requires std::derived_from<Color, mizu::Color>
void G2d::point(const Point<Color> &p) {
    point(p.pos, p.color);
}

template<typename Color>
    requires std::derived_from<Color, mizu::Color>
void G2d::line(const Line<Color> &l) {
    line(l.v0, l.v1, l.rot, l.color);
}

template<typename Color>
    requires std::derived_from<Color, mizu::Color>
void G2d::triangle(const Triangle<Color> &t) {
    triangle(t.v0, t.v1, t.v2, t.rot, t.color);
}
} // namespace mizu

ENUM_CLASS_ENABLE_BITOPS(mizu::ClearBit);

#endif // MIZU_G2D_HPP
