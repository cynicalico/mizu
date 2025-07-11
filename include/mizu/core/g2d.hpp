#ifndef MIZU_G2D_HPP
#define MIZU_G2D_HPP

#include <glm/vec2.hpp>
#include "gloo/context.hpp"
#include "gloo/texture.hpp"
#include "mizu/core/batcher.hpp"
#include "mizu/core/callback_mgr.hpp"
#include "mizu/core/color.hpp"
#include "mizu/core/texture.hpp"
#include "mizu/core/window.hpp"
#include "mizu/util/class_helpers.hpp"
#include "mizu/util/enum_class_helpers.hpp"
#include "mizu/util/shapes.hpp"

namespace mizu {
class G2d {
public:
    G2d(CallbackMgr &callbacks, gloo::Context &gl, Window *window);

    ~G2d();

    NO_COPY(G2d)
    NO_MOVE(G2d)

    std::unique_ptr<Texture> load_texture(
            const std::filesystem::path &path,
            gloo::MinFilter min_filter = gloo::MinFilter::Nearest,
            gloo::MagFilter mag_filter = gloo::MagFilter::Linear) const;

    std::unique_ptr<Texture> create_texture(
            glm::ivec2 size,
            gloo::MinFilter min_filter = gloo::MinFilter::Nearest,
            gloo::MagFilter mag_filter = gloo::MagFilter::Linear) const;

    bool vsync() const;
    void set_vsync(bool enabled);

    void clear(const Color &color, gloo::ClearBit mask = gloo::ClearBit::Color | gloo::ClearBit::Depth);

    void point(glm::vec2 pos, const Color &color);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void point(const Point<Color> &p);

    void line(glm::vec2 p0, glm::vec2 p1, glm::vec3 rot, const Color &color);
    void line(glm::vec2 p0, glm::vec2 p1, const Color &color);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void line(const Line<Color> &l);

    void fill_tri(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, glm::vec3 rot, const Color &color);
    void fill_tri(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void fill_tri(const Triangle<Color> &t);

    void fill_rect(glm::vec2 pos, glm::vec2 size, glm::vec3 rot, const Color &color);
    void fill_rect(glm::vec2 pos, glm::vec2 size, const Color &color);

    template<typename Color>
        requires std::derived_from<Color, mizu::Color>
    void fill_rect(const Rectangle<Color> &r);

    void
    texture(const Texture &t,
            glm::vec2 pos,
            glm::vec2 size,
            glm::vec4 region,
            glm::vec3 rot,
            const Color &color = rgb(0xffffff));
    void texture(const Texture &t, glm::vec2 pos, glm::vec4 region, glm::vec3 rot, const Color &color = rgb(0xffffff));
    void texture(const Texture &t, glm::vec2 pos, glm::vec3 rot, const Color &color = rgb(0xffffff));
    void texture(const Texture &t, glm::vec2 pos, glm::vec2 size, glm::vec3 rot, const Color &color = rgb(0xffffff));

private:
    gloo::Context &gl_;
    Window *window_;

    Batcher batcher_;

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();

    void pre_draw_();
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
void G2d::fill_tri(const Triangle<Color> &t) {
    fill_tri(t.v0, t.v1, t.v2, t.rot, t.color);
}

template<typename Color>
    requires std::derived_from<Color, mizu::Color>
void G2d::fill_rect(const Rectangle<Color> &r) {
    fill_rect(r.pos, r.size, r.rot, r.color);
}
} // namespace mizu

#endif // MIZU_G2D_HPP
