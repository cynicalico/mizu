#ifndef GLOO_TEXTURE_HPP
#define GLOO_TEXTURE_HPP

#include <glm/vec2.hpp>
#include "gloo/context.hpp"
#include "mizu/util/class_helpers.hpp"
#include "mizu/util/io.hpp"

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
    Texture(GladGLContext &gl, glm::ivec2 size, MinFilter min_filter, MagFilter mag_filter);

    ~Texture();

    NO_COPY(Texture)

    MOVE_CONSTRUCTOR(Texture);
    MOVE_ASSIGN_OP(Texture);

    void write_subimage(glm::ivec2 pos, glm::ivec2 size, const unsigned char *bytes);

private:
    GladGLContext &gl_;
};
} // namespace gloo

#endif // GLOO_TEXTURE_HPP
