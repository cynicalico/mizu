#ifndef GLOO_TEXTURE_HPP
#define GLOO_TEXTURE_HPP

#include "gloo/context.hpp"
#include "mizu/class_helpers.hpp"
#include "mizu/io.hpp"

namespace gloo {
enum class Scaling : GLenum {
    Smooth = GL_LINEAR,
    Retro = GL_NEAREST,
};

class Texture {
public:
    GLuint id{0};

    Texture(GladGLContext &gl, const mizu::PngData &data, Scaling scaling);
    ~Texture();

    NO_COPY(Texture)

    MOVE_CONSTRUCTOR(Texture);
    MOVE_ASSIGN_OP(Texture);

private:
    GladGLContext &gl_;
};
} // namespace gloo

#endif // GLOO_TEXTURE_HPP
