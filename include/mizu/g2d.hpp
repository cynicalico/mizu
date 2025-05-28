#ifndef MIZU_G2D_HPP
#define MIZU_G2D_HPP

#include "gloo/buffer.hpp"
#include "gloo/context.hpp"
#include "gloo/shader.hpp"
#include "mizu/callback_mgr.hpp"
#include "mizu/color.hpp"
#include "mizu/enum_class_helpers.hpp"

namespace mizu {
enum class ClearBit : GLenum {
    Color = GL_COLOR_BUFFER_BIT,
    Depth = GL_DEPTH_BUFFER_BIT,
    Stencil = GL_STENCIL_BUFFER_BIT
};

class G2d {
public:
    G2d(gloo::GlContext &gl, CallbackMgr &callbacks);

    ~G2d();

    G2d(const G2d &) = delete;
    G2d &operator=(const G2d &) = delete;

    G2d(G2d &&other) noexcept = delete;
    G2d &operator=(G2d &&other) = delete;

    void clear(const Color &color, ClearBit clear_bits);

    gloo::ShaderBuilder shader_builder() const;

    template<typename T>
    std::unique_ptr<gloo::Buffer<T>> buffer(std::size_t capacity) const;

private:
    gloo::GlContext &gl_;

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();
};

template<typename T>
std::unique_ptr<gloo::Buffer<T>> G2d::buffer(std::size_t capacity) const {
    return std::make_unique<gloo::Buffer<T>>(gl_.ctx, capacity);
}
} // namespace mizu

ENUM_CLASS_ENABLE_BITOPS(mizu::ClearBit);

#endif // MIZU_G2D_HPP
