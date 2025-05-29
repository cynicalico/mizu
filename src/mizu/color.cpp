#include "mizu/color.hpp"
#include <cmath>

namespace mizu {
glm::vec4 Rgba::gl_color() const {
    return {r / 255.0, g / 255.0, b / 255.0, a / 255.0};
}

Rgba::Rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
    : r(r), g(g), b(b), a(a) {}

Rgba rgb(std::uint32_t hex) {
    return {static_cast<std::uint8_t>(hex >> 16 & 0xff),
            static_cast<std::uint8_t>(hex >> 8 & 0xff),
            static_cast<std::uint8_t>(hex & 0xff),
            255};
}

Rgba rgba(std::uint64_t hex) {
    return {static_cast<std::uint8_t>(hex >> 24 & 0xff),
            static_cast<std::uint8_t>(hex >> 16 & 0xff),
            static_cast<std::uint8_t>(hex >> 8 & 0xff),
            static_cast<std::uint8_t>(hex & 0xff)};
}

Rgba rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    return {r, g, b, 255};
}

Rgba rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    return {r, g, b, a};
}

Rgba rgb_f(float r, float g, float b) {
    return {static_cast<std::uint8_t>(std::round(r * 255.0)),
            static_cast<std::uint8_t>(std::round(g * 255.0)),
            static_cast<std::uint8_t>(std::round(b * 255.0)),
            255};
}

Rgba rgba_f(float r, float g, float b, float a) {
    return {static_cast<std::uint8_t>(std::round(r * 255.0)),
            static_cast<std::uint8_t>(std::round(g * 255.0)),
            static_cast<std::uint8_t>(std::round(b * 255.0)),
            static_cast<std::uint8_t>(std::round(a * 255.0))};
}
} // namespace mizu
