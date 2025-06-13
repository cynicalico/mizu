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

namespace colorscheme {
// Windows XP console
namespace xp {
const Rgba BLACK = rgb(0, 0, 0);
const Rgba RED = rgb(128, 0, 0);
const Rgba GREEN = rgb(0, 128, 0);
const Rgba YELLOW = rgb(128, 128, 0);
const Rgba BLUE = rgb(0, 0, 128);
const Rgba MAGENTA = rgb(128, 0, 128);
const Rgba CYAN = rgb(0, 128, 128);
const Rgba WHITE = rgb(192, 192, 192);

const Rgba B_BLACK = rgb(128, 128, 128);
const Rgba B_RED = rgb(255, 0, 0);
const Rgba B_GREEN = rgb(0, 255, 0);
const Rgba B_YELLOW = rgb(255, 255, 0);
const Rgba B_BLUE = rgb(0, 0, 255);
const Rgba B_MAGENTA = rgb(255, 0, 255);
const Rgba B_CYAN = rgb(0, 255, 255);
const Rgba B_WHITE = rgb(255, 255, 255);
} // namespace xp

// Ubuntu terminal
namespace ansi {
const Rgba BLACK = rgb(1, 1, 1);
const Rgba RED = rgb(222, 56, 43);
const Rgba GREEN = rgb(57, 181, 74);
const Rgba YELLOW = rgb(255, 199, 6);
const Rgba BLUE = rgb(0, 111, 184);
const Rgba MAGENTA = rgb(118, 38, 113);
const Rgba CYAN = rgb(44, 181, 233);
const Rgba WHITE = rgb(204, 204, 204);

const Rgba B_BLACK = rgb(128, 128, 128);
const Rgba B_RED = rgb(255, 0, 0);
const Rgba B_GREEN = rgb(0, 255, 0);
const Rgba B_YELLOW = rgb(255, 255, 0);
const Rgba B_BLUE = rgb(0, 0, 255);
const Rgba B_MAGENTA = rgb(255, 0, 255);
const Rgba B_CYAN = rgb(0, 255, 255);
const Rgba B_WHITE = rgb(255, 255, 255);
} // namespace ansi

// From the new Windows Terminal
namespace campbell {
const Rgba BLACK = rgb(0x0c0c0c);
const Rgba RED = rgb(0xc50f1f);
const Rgba GREEN = rgb(0x13a10e);
const Rgba YELLOW = rgb(0xc19c00);
const Rgba BLUE = rgb(0x0037da);
const Rgba MAGENTA = rgb(0x881798);
const Rgba CYAN = rgb(0x3a96dd);
const Rgba WHITE = rgb(0xcccccc);

const Rgba B_BLACK = rgb(0x767676);
const Rgba B_RED = rgb(0xe74856);
const Rgba B_GREEN = rgb(0x16c60c);
const Rgba B_YELLOW = rgb(0xf9f1a5);
const Rgba B_BLUE = rgb(0x3b78ff);
const Rgba B_MAGENTA = rgb(0xb4009e);
const Rgba B_CYAN = rgb(0x61d6d6);
const Rgba B_WHITE = rgb(0xf2f2f2);
} // namespace campbell
} // namespace colorscheme
} // namespace mizu

#endif // MIZU_COLOR_HPP
