#include "mizu/gui/control.hpp"

namespace mizu::gui {
Button::Button(const ButtonParams &params)
    : Node(params.grow),
      font(params.font),
      text(params.text),
      text_pad(params.text_pad),
      bg_color(params.bg_color),
      fg_color(params.fg_color),
      border(params.border),
      onclick(params.onclick) {}

void Button::resize(const glm::vec2 &max_size_hint) {
    const auto text_size = font->calc_size(text);

    switch (grow) {
    case Grow::Hori: {
        size.x = max_size_hint.x != UNDEFINED_SIZE ? max_size_hint.x
                                                   : text_size.x + text_pad.left + text_pad.right + 2 * border_size_();
        size.y = text_size.y + text_pad.top + text_pad.bottom + 2 * border_size_();
    } break;
    case Grow::Vert: {
        size.x = text_size.x + text_pad.left + text_pad.right + 2 * border_size_();
        size.y = max_size_hint.y != UNDEFINED_SIZE ? max_size_hint.y
                                                   : text_size.y + text_pad.top + text_pad.bottom + 2 * border_size_();
    } break;
    case Grow::Both: {
        size.x = max_size_hint.x != UNDEFINED_SIZE ? max_size_hint.x
                                                   : text_size.x + text_pad.left + text_pad.right + 2 * border_size_();
        size.y = max_size_hint.y != UNDEFINED_SIZE ? max_size_hint.y
                                                   : text_size.y + text_pad.top + text_pad.bottom + 2 * border_size_();
    } break;
    case Grow::None: {
        size.x = text_size.x + text_pad.left + text_pad.right + 2 * border_size_();
        size.y = text_size.y + text_pad.top + text_pad.bottom + 2 * border_size_();
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
    if (bg_color.a > 0)
        g2d.fill_rect({bbox.x, bbox.y}, {size.x, size.y}, bg_color);

    if (border) {
        if (std::holds_alternative<PxBorder>(*border)) {
            const auto border_color = std::get<PxBorder>(*border).color;
            g2d.line({bbox.x, bbox.y}, {bbox.z - border_size_(), bbox.y}, border_color);
            g2d.line(
                    {bbox.z - border_size_(), bbox.y},
                    {bbox.z - border_size_(), bbox.w - border_size_()},
                    border_color);
            g2d.line(
                    {bbox.z - border_size_(), bbox.w - border_size_()},
                    {bbox.x, bbox.w - border_size_()},
                    border_color);
            g2d.line({bbox.x, bbox.w - border_size_()}, {bbox.x, bbox.y}, border_color);
        } else {
            throw std::runtime_error("not yet implemented");
        }
    }

    const auto text_size = font->calc_size(text);
    font->draw(
            text,
            {std::round(bbox.x + (size.x - text_size.x) / 2),
             std::round(bbox.y + (size.y - text_size.y) / 2) + font->pen_offset()},
            fg_color);
}

float Button::border_size_() const {
    if (border.has_value()) {
        if (std::holds_alternative<PxBorder>(*border))
            return 1.0f;
        throw std::runtime_error("not yet implemented");
    }
    return 0.0f;
}
} // namespace mizu::gui
