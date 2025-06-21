#include "mizu/gui/gui.hpp"
#include <unordered_set>

namespace mizu::gui {
constexpr auto UNDEFINED_SIZE = std::numeric_limits<float>::max();
constexpr auto UNDEFINED_SIZE_V = glm::vec2{UNDEFINED_SIZE, UNDEFINED_SIZE};

VStack::VStack(const Padding outer_pad, float inner_pad)
    : outer_pad(outer_pad), inner_pad(inner_pad) {}

glm::vec2 VStack::calc_size(const glm::vec2 &max_size_hint) {
    switch (grow) {
    case Grow::Hori: size = {max_size_hint.x, UNDEFINED_SIZE}; break;
    case Grow::Vert: size = {UNDEFINED_SIZE, max_size_hint.y}; break;
    case Grow::Both: size = {max_size_hint.x, max_size_hint.y}; break;
    case Grow::None: size = {UNDEFINED_SIZE, UNDEFINED_SIZE}; break;
    }

    glm::vec2 child_max_size_hint = size;
    if (child_max_size_hint.x != UNDEFINED_SIZE) {
        child_max_size_hint.x -= 2 * BORDER_SIZE;
        child_max_size_hint.x -= outer_pad.left + outer_pad.right;
    }
    if (child_max_size_hint.y != UNDEFINED_SIZE) {
        child_max_size_hint.y -= 2 * BORDER_SIZE;
        child_max_size_hint.y -= outer_pad.top + outer_pad.bottom;
        child_max_size_hint.y -= (children.size() - 1) * inner_pad;
    }

    if (grow == Grow::Hori || grow == Grow::None) {
        for (const auto &child: children)
            child->calc_size(child_max_size_hint);
    } else {
        // Check which elements won't grow vertically
        auto growing_children = std::unordered_set<std::size_t>();
        for (std::size_t i = 0; i < children.size(); ++i) {
            if (children[i]->grow == Grow::Hori || children[i]->grow == Grow::None) {
                const auto child_size = children[i]->calc_size(UNDEFINED_SIZE_V);
                if (child_max_size_hint.y != UNDEFINED_SIZE)
                    child_max_size_hint.y -= child_size.y;
            } else {
                growing_children.insert(i);
            }
        }
        if (!growing_children.empty()) {
            if (child_max_size_hint.y != UNDEFINED_SIZE)
                child_max_size_hint.y /= growing_children.size();

            // Calculate the rest in the remaining space
            for (const std::size_t &i: growing_children)
                children[i]->calc_size(child_max_size_hint);
        }
    }

    if (size.x == UNDEFINED_SIZE) {
        size.x = 0;
        for (const auto &child: children)
            size.x = std::max(size.x, child->size.x);
        size.x += 2 * BORDER_SIZE;
        size.x += outer_pad.left + outer_pad.right;

        child_max_size_hint.x = size.x - 2 * BORDER_SIZE - outer_pad.left - outer_pad.right;
        // One more pass with our newly calculated size
        // because something inside may be horizontally growable
        for (const auto &child: children)
            child->calc_size(child_max_size_hint);
    }

    if (size.y == UNDEFINED_SIZE) {
        size.y = 0;
        for (std::size_t i = 0; i < children.size(); ++i)
            size.y += children[i]->size.y;
        size.y += 2 * BORDER_SIZE;
        size.y += outer_pad.top + outer_pad.bottom;
        size.y += (children.size() - 1) * inner_pad;
    }

    return size;
}

void VStack::draw(G2d &g2d, glm::vec2 pos) const {
    g2d.line(pos, {pos.x + size.x - BORDER_SIZE, pos.y}, rgb(0xffffff));
    g2d.line({pos.x + size.x - BORDER_SIZE, pos.y}, {pos.x + size.x - 1, pos.y + size.y - BORDER_SIZE}, rgb(0xffffff));
    g2d.line({pos.x + size.x - BORDER_SIZE, pos.y + size.y - 1}, {pos.x, pos.y + size.y - BORDER_SIZE}, rgb(0xffffff));
    g2d.line({pos.x, pos.y + size.y - BORDER_SIZE}, pos, rgb(0xffffff));

    glm::vec2 next_child_pos{pos.x + BORDER_SIZE + outer_pad.left, pos.y + BORDER_SIZE + outer_pad.top};
    for (const auto &child: children) {
        const auto center_offset = (size.x - 2 * BORDER_SIZE - outer_pad.left - outer_pad.right - child->size.x) / 2;
        glm::vec2 draw_pos{next_child_pos.x + center_offset, next_child_pos.y};
        child->draw(g2d, draw_pos);

        next_child_pos.y += child->size.y;
        next_child_pos.y += inner_pad;
    }
}

HStack::HStack(const Padding outer_pad, float inner_pad)
    : outer_pad(outer_pad), inner_pad(inner_pad) {}

glm::vec2 HStack::calc_size(const glm::vec2 &max_size_hint) {
    switch (grow) {
    case Grow::Hori: size = {max_size_hint.x, UNDEFINED_SIZE}; break;
    case Grow::Vert: size = {UNDEFINED_SIZE, max_size_hint.y}; break;
    case Grow::Both: size = {max_size_hint.x, max_size_hint.y}; break;
    case Grow::None: size = {UNDEFINED_SIZE, UNDEFINED_SIZE}; break;
    }

    glm::vec2 child_max_size_hint = size;
    if (child_max_size_hint.x != UNDEFINED_SIZE) {
        child_max_size_hint.x -= 2 * BORDER_SIZE;
        child_max_size_hint.x -= outer_pad.left + outer_pad.right;
        child_max_size_hint.x -= (children.size() - 1) * inner_pad;
    }
    if (child_max_size_hint.y != UNDEFINED_SIZE) {
        child_max_size_hint.y -= 2 * BORDER_SIZE;
        child_max_size_hint.y -= outer_pad.top + outer_pad.bottom;
    }

    if (grow == Grow::Vert || grow == Grow::None) {
        for (const auto &child: children)
            child->calc_size(child_max_size_hint);
    } else {
        // Check which elements won't grow horizontally
        auto growing_children = std::unordered_set<std::size_t>();
        for (std::size_t i = 0; i < children.size(); ++i) {
            if (children[i]->grow == Grow::Vert || children[i]->grow == Grow::None) {
                const auto child_size = children[i]->calc_size({UNDEFINED_SIZE, child_max_size_hint.y});
                if (child_max_size_hint.x != UNDEFINED_SIZE)
                    child_max_size_hint.x -= child_size.x;
            } else {
                growing_children.insert(i);
            }
        }
        if (!growing_children.empty()) {
            if (child_max_size_hint.x != UNDEFINED_SIZE)
                child_max_size_hint.x /= growing_children.size();

            // Calculate the rest in the remaining space
            for (const std::size_t &i: growing_children)
                children[i]->calc_size({child_max_size_hint});
        }
    }

    if (size.x == UNDEFINED_SIZE) {
        size.x = 0;
        for (const auto &child: children)
            size.x += child->size.x;
        size.x += 2 * BORDER_SIZE;
        size.x += outer_pad.left + outer_pad.right;
        size.x += (children.size() - 1) * inner_pad;
    }

    if (size.y == UNDEFINED_SIZE) {
        size.y = 0;
        for (const auto &child: children)
            size.y = std::max(size.y, child->size.y);
        size.y += 2 * BORDER_SIZE;
        size.y += outer_pad.top + outer_pad.bottom;

        child_max_size_hint.y = size.y - 2 * BORDER_SIZE - outer_pad.top - outer_pad.bottom;
        // One more pass with our newly calculated size
        // because something inside may be vertically growable
        for (const auto &child: children)
            child->calc_size(child_max_size_hint);
    }

    return size;
}

void HStack::draw(G2d &g2d, glm::vec2 pos) const {
    g2d.line(pos, {pos.x + size.x - BORDER_SIZE, pos.y}, rgb(0xffffff));
    g2d.line(
            {pos.x + size.x - BORDER_SIZE, pos.y},
            {pos.x + size.x - BORDER_SIZE, pos.y + size.y - BORDER_SIZE},
            rgb(0xffffff));
    g2d.line(
            {pos.x + size.x - BORDER_SIZE, pos.y + size.y - BORDER_SIZE},
            {pos.x, pos.y + size.y - BORDER_SIZE},
            rgb(0xffffff));
    g2d.line({pos.x, pos.y + size.y - BORDER_SIZE}, pos, rgb(0xffffff));

    glm::vec2 next_child_pos{pos.x + BORDER_SIZE + outer_pad.left, pos.y + BORDER_SIZE + outer_pad.top};
    for (const auto &child: children) {
        const auto center_offset = (size.y - 2 * BORDER_SIZE - outer_pad.top - outer_pad.bottom - child->size.y) / 2;
        glm::vec2 draw_pos{next_child_pos.x, next_child_pos.y + center_offset};
        child->draw(g2d, draw_pos);

        next_child_pos.x += child->size.x;
        next_child_pos.x += inner_pad;
    }
}

Button::Button(CodePage437 *font, const std::string &text, float text_scale)
    : font_(font), text_(text), text_scale_(text_scale) {}

glm::vec2 Button::calc_size(const glm::vec2 &max_size_hint) {
    const auto text_size = font_->calculate_size(text_, text_scale_);

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

    return size;
}

void Button::draw(G2d &g2d, glm::vec2 pos) const {
    g2d.line(pos, {pos.x + size.x - BORDER_SIZE, pos.y}, rgb(0x00ff00));
    g2d.line(
            {pos.x + size.x - BORDER_SIZE, pos.y},
            {pos.x + size.x - BORDER_SIZE, pos.y + size.y - BORDER_SIZE},
            rgb(0x00ff00));
    g2d.line(
            {pos.x + size.x - BORDER_SIZE, pos.y + size.y - BORDER_SIZE},
            {pos.x, pos.y + size.y - BORDER_SIZE},
            rgb(0x00ff00));
    g2d.line({pos.x, pos.y + size.y - BORDER_SIZE}, pos, rgb(0x00ff00));

    const auto text_size = font_->calculate_size(text_, text_scale_);
    font_->draw(text_, {pos.x + (size.x - text_size.x) / 2, pos.y + (size.y - text_size.y) / 2}, text_scale_);
}
} // namespace mizu::gui
