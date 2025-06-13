#ifndef MIZU_SHAPES_HPP
#define MIZU_SHAPES_HPP

#include <glm/vec2.hpp>
#include "mizu/core/color.hpp"

namespace mizu {
template<typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Point {
    glm::vec2 pos;
    Color color{};

    Point(const glm::vec2 pos, Color color)
        : pos(pos), color(color) {}
};

template<typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Line {
    glm::vec2 v0;
    glm::vec2 v1;
    glm::vec3 rot;
    Color color;

    Line(const glm::vec2 v0, const glm::vec2 v1, const glm::vec3 rot, Color color)
        : v0(v0), v1(v1), rot(rot), color(color) {}
};

template<typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Triangle {
    glm::vec2 v0;
    glm::vec2 v1;
    glm::vec2 v2;
    glm::vec3 rot;
    Color color;

    Triangle(const glm::vec2 v0, const glm::vec2 v1, const glm::vec2 v2, const glm::vec3 rot, Color color)
        : v0(v0), v1(v1), v2(v2), rot(rot), color(color) {}
};

template<typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Rectangle {
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec3 rot;
    Color color;

    Rectangle(const glm::vec2 pos, const glm::vec2 size, const glm::vec3 rot, Color color)
        : pos(pos), size(size), rot(rot), color(color) {}
};
} // namespace mizu

#endif // MIZU_SHAPES_HPP
