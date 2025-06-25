#include "mizu/gui/control.hpp"

namespace mizu::gui {
Button::Button(const ButtonParams &params)
    : font(params.font), text(params.text), text_scale(params.text_scale), onclick(params.onclick) {}

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

glm::tvec2<Id> Button::update(InputMgr &input, Id mouse_captured, Id keyboard_captured) {
    if (mouse_captured != NO_CAPTURE && mouse_captured != id)
        return {NO_CAPTURE, NO_CAPTURE};

    hovered_ = input.mouse_x() >= bbox.x && input.mouse_x() < bbox.z && input.mouse_y() >= bbox.y &&
               input.mouse_y() < bbox.w;

    // If the user is in bounds and clicked, process and capture
    if (!primed_ && hovered_ && input.pressed(MouseButton::Left)) {
        primed_ = true;
    }

    // If the user has release left click, process and uncapture
    else if (primed_ && input.released(MouseButton::Left)) {
        if (hovered_)
            onclick(this);
        primed_ = false;
    }

    if (primed_)
        return {id, NO_CAPTURE};
    return {NO_CAPTURE, NO_CAPTURE};
}

void Button::draw(G2d &g2d) const {
    Rgba border_color = rgb(0x00ff00);
    if (hovered_)
        border_color = rgb(0xff00ff);

    g2d.line({bbox.x, bbox.y}, {bbox.z - BORDER_SIZE, bbox.y}, border_color);
    g2d.line({bbox.z - BORDER_SIZE, bbox.y}, {bbox.z - BORDER_SIZE, bbox.w - BORDER_SIZE}, border_color);
    g2d.line({bbox.z - BORDER_SIZE, bbox.w - BORDER_SIZE}, {bbox.x, bbox.w - BORDER_SIZE}, border_color);
    g2d.line({bbox.x, bbox.w - BORDER_SIZE}, {bbox.x, bbox.y}, border_color);

    const auto text_size = font->calculate_size(text, text_scale);
    font->draw(text, {bbox.x + (size.x - text_size.x) / 2, bbox.y + (size.y - text_size.y) / 2}, text_scale);
}
} // namespace mizu::gui
