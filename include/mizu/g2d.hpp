#ifndef MIZU_G2D_HPP
#define MIZU_G2D_HPP

#include "gloo/context.hpp"
#include "mizu/callback_mgr.hpp"
#include "mizu/color.hpp"

namespace mizu {
class G2d {
public:
    G2d(gloo::GlContext &gl, CallbackMgr &callbacks);

    ~G2d();

    G2d(const G2d &) = delete;
    G2d &operator=(const G2d &) = delete;

    G2d(G2d &&other) noexcept = delete;
    G2d &operator=(G2d &&other) = delete;

    void set_clear_color(const Color &c);
    Rgba clear_color() const;

private:
    gloo::GlContext &gl_;

    glm::vec4 clear_color_;

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();

    void draw_();
};
} // namespace mizu

#endif // MIZU_G2D_HPP
