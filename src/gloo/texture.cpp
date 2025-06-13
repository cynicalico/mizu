#include "gloo/texture.hpp"
#include "mizu/log.hpp"

namespace gloo {
Texture::Texture(GladGLContext &gl, const mizu::PngData &data, MinFilter min_filter, MagFilter mag_filter)
    : gl_(gl) {
    gl_.GenTextures(1, &id);
    CHECK_GL_ERROR(gl_, GenTextures);
    MIZU_LOG_TRACE("Created texture id={}", id);

    gl_.ActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR(gl_, BindTexture);
    gl_.BindTexture(GL_TEXTURE_2D, id);
    CHECK_GL_ERROR(gl_, BindTexture);

    gl_.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR(gl_, TexParameteri);
    gl_.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECK_GL_ERROR(gl_, TexParameteri);
    gl_.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, unwrap(min_filter));
    CHECK_GL_ERROR(gl_, TexParameteri);
    gl_.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, unwrap(mag_filter));
    CHECK_GL_ERROR(gl_, TexParameteri);

    gl_.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.bytes);
    CHECK_GL_ERROR(gl_, TexImage2D);
    gl_.GenerateMipmap(GL_TEXTURE_2D);
    CHECK_GL_ERROR(gl_, GenerateMipmap);

    gl_.BindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR(gl_, BindTexture);
}

Texture::~Texture() {
    if (id != 0) {
        gl_.DeleteTextures(1, &id);
        CHECK_GL_ERROR(gl_, DeleteTextures);
        MIZU_LOG_TRACE("Deleted texture id={}", id);
    }
}

MOVE_CONSTRUCTOR_IMPL(Texture)
    : id(other.id), gl_(other.gl_) {
    other.id = 0;
}

MOVE_ASSIGN_OP_IMPL(Texture) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        gl_ = other.gl_;
    }
    return *this;
}
} // namespace gloo
