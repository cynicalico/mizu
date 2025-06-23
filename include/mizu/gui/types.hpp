#ifndef MIZU_GUI_TYPES_HPP
#define MIZU_GUI_TYPES_HPP

#include <variant>
#include "mizu/core/color.hpp"

namespace mizu::gui {
constexpr auto UNDEFINED_SIZE = std::numeric_limits<float>::max();
constexpr auto UNDEFINED_SIZE_V = glm::vec2{UNDEFINED_SIZE, UNDEFINED_SIZE};

constexpr float BORDER_SIZE = 1.0f;

enum class Grow { Hori, Vert, Both, None };

struct PxBorder {
    Rgba color;
};

using Border = std::variant<PxBorder>;

struct Padding {
    float left, right, top, bottom;

    Padding(float left, float right, float top, float bottom)
        : left(left), right(right), top(top), bottom(bottom) {}

    Padding(float hori, float vert)
        : left(hori), right(hori), top(vert), bottom(vert) {}

    explicit Padding(float all)
        : left(all), right(all), top(all), bottom(all) {}
};
} // namespace mizu::gui

#endif // MIZU_GUI_TYPES_HPP
