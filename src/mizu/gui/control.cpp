#include "mizu/gui/control.hpp"

namespace mizu::gui {
Button::Button(ButtonParams params)
    : font(params.font), text(params.text), text_scale(params.text_scale) {}

void Button::resize(const glm::vec2 &max_size_hint) {
    const auto text_size = font->calculate_size(text, text_scale);

    switch (grow) {
    case Grow::Hori: {
        size.x = max_size_hint.x != UNDEFINED_SIZE ? max_size_hint.x : text_size.x + 2 + 2 * BORDER_SIZE;
        size.y = text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    case Grow::Vert: {
        size.x = text_size.x + 2 + 2 * BORDER_SIZE;
        size.y = max_size_hint.y != UNDEFINED_SIZE ? max_size_hint.y : text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    case Grow::Both: {
        size.x = max_size_hint.x != UNDEFINED_SIZE ? max_size_hint.x : text_size.x + 2 + 2 * BORDER_SIZE;
        size.y = max_size_hint.y != UNDEFINED_SIZE ? max_size_hint.y : text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    case Grow::None: {
        size.x = text_size.x + 2 + 2 * BORDER_SIZE;
        size.y = text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    }
}

void Button::calc_bbox(glm::vec2 pos) {
    bbox = {pos.x, pos.y, pos.x + size.x, pos.y + size.y};
}

void Button::draw(G2d &g2d) const {
    g2d.line({bbox.x, bbox.y}, {bbox.z - BORDER_SIZE, bbox.y}, rgb(0x00ff00));
    g2d.line({bbox.z - BORDER_SIZE, bbox.y}, {bbox.z - BORDER_SIZE, bbox.w - BORDER_SIZE}, rgb(0x00ff00));
    g2d.line({bbox.z - BORDER_SIZE, bbox.w - BORDER_SIZE}, {bbox.x, bbox.w - BORDER_SIZE}, rgb(0x00ff00));
    g2d.line({bbox.x, bbox.w - BORDER_SIZE}, {bbox.x, bbox.y}, rgb(0x00ff00));

    const auto text_size = font->calculate_size(text, text_scale);
    font->draw(text, {bbox.x + (size.x - text_size.x) / 2, bbox.y + (size.y - text_size.y) / 2}, text_scale);
}
} // namespace mizu::gui
