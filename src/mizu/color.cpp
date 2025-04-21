#include "mizu/color.hpp"

namespace mizu {
glm::vec4 Rgba::gl_color() const {
    return {r / 255.0, g / 255.0, b / 255.0, a / 255.0};
}

Rgba::Rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) : r(r), g(g), b(b), a(a) {}

Rgba rgb(std::uint32_t hex) {
    return Rgba((hex >> 16) & 0xff, (hex >> 8) & 0xff, hex & 0xff, 255);
}

Rgba rgba(std::uint64_t hex) {
    return Rgba((hex >> 24) & 0xff, (hex >> 16) & 0xff, (hex >> 8) & 0xff, hex & 0xff);
}

Rgba rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    return Rgba(r, g, b, 255);
}

Rgba rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    return Rgba(r, g, b, a);
}
} // namespace mizu
