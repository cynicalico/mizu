#ifndef MIZU_SHAPES_HPP
#define MIZU_SHAPES_HPP

#include <glm/vec2.hpp>
#include "mizu/color.hpp"

namespace mizu {
template<typename T = float, typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Point {
    glm::tvec2<T> pos;
    Color color{};
};

template<typename T = float, typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Line {
    glm::tvec2<T> v0;
    glm::tvec2<T> v1;
    Color color;
};

template<typename T = float, typename Color = Rgba>
    requires std::derived_from<Color, mizu::Color>
struct Triangle {
    glm::tvec2<T> v0;
    glm::tvec2<T> v1;
    glm::tvec2<T> v2;
    Color color;
};
} // namespace mizu

#endif // MIZU_SHAPES_HPP
