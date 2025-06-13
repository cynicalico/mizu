#ifndef GLOO_TEXTURE_HPP
#define GLOO_TEXTURE_HPP

#include "gloo/context.hpp"
#include "mizu/class_helpers.hpp"
#include "mizu/io.hpp"

namespace gloo {
enum class MinFilter : GLenum {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
};

enum class MagFilter : GLenum {
    Nearest = GL_NEAREST,
    Linear = GL_LINEAR,
};

class Texture {
public:
    GLuint id{0};

    Texture(GladGLContext &gl, const mizu::PngData &data, MinFilter min_filter, MagFilter mag_filter);
    ~Texture();

    NO_COPY(Texture)

    MOVE_CONSTRUCTOR(Texture);
    MOVE_ASSIGN_OP(Texture);

private:
    GladGLContext &gl_;
};
} // namespace gloo

#endif // GLOO_TEXTURE_HPP
