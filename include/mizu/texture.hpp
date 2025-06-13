#ifndef MIZU_TEXTURE_HPP
#define MIZU_TEXTURE_HPP

#include <filesystem>
#include "gloo/context.hpp"
#include "gloo/texture.hpp"
#include "mizu/io.hpp"

namespace mizu {
class Texture {
public:
    Texture(gloo::Context &gl, const std::filesystem::path &path, gloo::MinFilter min_filter, gloo::MagFilter mag_filter
    );

    ~Texture() = default;

    NO_COPY(Texture)

    MOVE_CONSTRUCTOR(Texture);
    MOVE_ASSIGN_OP(Texture);

    GLuint id() const;

    float width() const;
    float height() const;

    float s(float x) const;
    float t(float y) const;

private:
    gloo::Context &gl_;

    PngData data_;
    float px_x_;
    float px_y_;

    gloo::Texture handle_;
};
} // namespace mizu

#endif // MIZU_TEXTURE_HPP
