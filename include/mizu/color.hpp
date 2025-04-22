#ifndef MIZU_COLOR_HPP
#define MIZU_COLOR_HPP

#include "glm/vec4.hpp"

namespace mizu {
class Color {
public:
    virtual ~Color() = default;
    virtual glm::vec4 gl_color() const = 0;
};

class Rgba final : public Color {
public:
    std::uint8_t r, g, b, a;

    glm::vec4 gl_color() const override;

    friend Rgba rgb(std::uint32_t hex);
    friend Rgba rgba(std::uint64_t hex);

    friend Rgba rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
    friend Rgba rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

    friend Rgba rgb_f(float r, float g, float b);
    friend Rgba rgba_f(float r, float g, float b, float a);

private:
    Rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
};

Rgba rgb(std::uint32_t hex);
Rgba rgba(std::uint64_t hex);

Rgba rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
Rgba rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

Rgba rgb_f(float r, float g, float b);
Rgba rgba_f(float r, float g, float b, float a);
} // namespace mizu

#endif // MIZU_COLOR_HPP
