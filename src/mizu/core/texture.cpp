#include "mizu/core/texture.hpp"

namespace mizu {
Texture::Texture(
        gloo::Context &gl, const std::filesystem::path &path, gloo::MinFilter min_filter, gloo::MagFilter mag_filter)
    : gl_(gl),
      data_(read_image_data(path).value_or(PngData(0, 0, 0, 0))),
      px_x_(1.0f / static_cast<float>(data_.width)),
      px_y_(1.0f / static_cast<float>(data_.height)),
      handle_(gl_.ctx, data_, min_filter, mag_filter) {}

Texture::Texture(gloo::Context &gl, glm::ivec2 size, gloo::MinFilter min_filter, gloo::MagFilter mag_filter)
    : gl_(gl),
      data_(PngData(size.x, size.y, 4, size.x * 4)),
      px_x_(1.0f / static_cast<float>(data_.width)),
      px_y_(1.0f / static_cast<float>(data_.height)),
      handle_(gl_.ctx, size, min_filter, mag_filter) {}

MOVE_CONSTRUCTOR_IMPL(Texture)
    : gl_(other.gl_),
      data_(std::move(other.data_)),
      px_x_(other.px_x_),
      px_y_(other.px_y_),
      handle_(std::move(other.handle_)) {}

MOVE_ASSIGN_OP_IMPL(Texture) {
    if (this != &other) {
        gl_ = other.gl_;

        data_ = std::move(other.data_);

        px_x_ = other.px_x_;
        other.px_x_ = 0;

        px_y_ = other.px_y_;
        other.px_y_ = 0;

        handle_ = std::move(other.handle_);
    }
    return *this;
}

GLuint Texture::id() const {
    return handle_.id;
}

float Texture::width() const {
    return static_cast<float>(data_.width);
}

float Texture::height() const {
    return static_cast<float>(data_.height);
}

float Texture::s(float x) const {
    return x * px_x_;
}

float Texture::t(float y) const {
    return y * px_y_;
}

void Texture::write_subimage(glm::ivec2 pos, glm::ivec2 size, const unsigned char *bytes) {
    // TODO: Make data_ reflect the change
    handle_.write_subimage(pos, size, bytes);
}
} // namespace mizu
