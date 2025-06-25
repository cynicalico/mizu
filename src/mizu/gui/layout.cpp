#include "mizu/gui/layout.hpp"

namespace mizu::gui {
VStack::VStack(const VStackParams &params)
    : border(params.border), outer_pad(params.outer_pad), inner_pad(params.inner_pad) {}

void VStack::resize(const glm::vec2 &max_size_hint) {
    switch (grow) {
    case Grow::Hori: size = {max_size_hint.x, UNDEFINED_SIZE}; break;
    case Grow::Vert: size = {UNDEFINED_SIZE, max_size_hint.y}; break;
    case Grow::Both: size = {max_size_hint.x, max_size_hint.y}; break;
    case Grow::None: size = {UNDEFINED_SIZE, UNDEFINED_SIZE}; break;
    }

    glm::vec2 child_max_size_hint = size;
    if (child_max_size_hint.x != UNDEFINED_SIZE) {
        child_max_size_hint.x -= 2 * border_size_();
        child_max_size_hint.x -= outer_pad.left + outer_pad.right;
    }
    if (child_max_size_hint.y != UNDEFINED_SIZE) {
        child_max_size_hint.y -= 2 * border_size_();
        child_max_size_hint.y -= outer_pad.top + outer_pad.bottom;
        child_max_size_hint.y -= (static_cast<float>(children.size()) - 1) * inner_pad;
    }

    if (grow == Grow::Hori || grow == Grow::None) {
        for (const auto &child: children)
            child->resize(child_max_size_hint);
    } else {
        // Check which elements won't grow vertically
        auto growing_children = std::unordered_set<std::size_t>();
        for (const auto &[i, child]: children | std::views::enumerate) {
            if (child->grow == Grow::Hori || child->grow == Grow::None) {
                child->resize(UNDEFINED_SIZE_V);
                if (child_max_size_hint.y != UNDEFINED_SIZE)
                    child_max_size_hint.y -= child->size.y;
            } else {
                growing_children.insert(i);
            }
        }
        if (!growing_children.empty()) {
            if (child_max_size_hint.y != UNDEFINED_SIZE)
                child_max_size_hint.y /= static_cast<float>(growing_children.size());

            // Calculate the rest in the remaining space
            for (const std::size_t &i: growing_children)
                children[i]->resize(child_max_size_hint);
        }
    }

    if (size.x == UNDEFINED_SIZE) {
        size.x = std::ranges::max_element(
                         children, std::ranges::less{}, [](const auto &a) -> float { return a->size.x; })
                         ->get()
                         ->size.x;
        size.x += 2 * border_size_();
        size.x += outer_pad.left + outer_pad.right;

        child_max_size_hint.x = size.x - 2 * border_size_() - outer_pad.left - outer_pad.right;
        // One more pass with our newly calculated size
        // because something inside may be horizontally growable
        for (const auto &child: children)
            child->resize(child_max_size_hint);
    }

    if (size.y == UNDEFINED_SIZE) {
        size.y = std::ranges::fold_left(children, 0.0f, [](float sum, const auto &a) { return sum + a->size.y; });
        size.y += 2 * border_size_();
        size.y += outer_pad.top + outer_pad.bottom;
        size.y += (static_cast<float>(children.size()) - 1) * inner_pad;
    }
}

void VStack::calc_bbox(glm::vec2 pos) {
    bbox = {pos.x, pos.y, pos.x + size.x, pos.y + size.y};

    glm::vec2 next_child_pos{pos.x + border_size_() + outer_pad.left, pos.y + border_size_() + outer_pad.top};
    for (const auto &child: children) {
        const auto center_offset = (size.x - 2 * border_size_() - outer_pad.left - outer_pad.right - child->size.x) / 2;
        child->calc_bbox({next_child_pos.x + center_offset, next_child_pos.y});

        next_child_pos.y += child->size.y;
        next_child_pos.y += inner_pad;
    }
}

glm::tvec2<Id> VStack::update(InputMgr &input, Id mouse_captured, Id keyboard_captured) {
    glm::tvec2<Id> new_capture = {NO_CAPTURE, NO_CAPTURE};
    for (const auto &child: children) {
        const auto child_capture = child->update(input, mouse_captured, keyboard_captured);
        if (child_capture.x != NO_CAPTURE)
            new_capture.x = child_capture.x;
        if (child_capture.y != NO_CAPTURE)
            new_capture.y = child_capture.y;
    }
    return new_capture;
}

void VStack::draw(G2d &g2d) const {
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

    for (const auto &child: children)
        child->draw(g2d);
}

float VStack::border_size_() const {
    if (border.has_value()) {
        if (std::holds_alternative<PxBorder>(*border))
            return 1.0f;
        throw std::runtime_error("not yet implemented");
    }
    return 0.0f;
}

HStack::HStack(const HStackParams &params)
    : border(params.border), outer_pad(params.outer_pad), inner_pad(params.inner_pad) {}

void HStack::resize(const glm::vec2 &max_size_hint) {
    switch (grow) {
    case Grow::Hori: size = {max_size_hint.x, UNDEFINED_SIZE}; break;
    case Grow::Vert: size = {UNDEFINED_SIZE, max_size_hint.y}; break;
    case Grow::Both: size = {max_size_hint.x, max_size_hint.y}; break;
    case Grow::None: size = {UNDEFINED_SIZE, UNDEFINED_SIZE}; break;
    }

    glm::vec2 child_max_size_hint = size;
    if (child_max_size_hint.x != UNDEFINED_SIZE) {
        child_max_size_hint.x -= 2 * border_size_();
        child_max_size_hint.x -= outer_pad.left + outer_pad.right;
        child_max_size_hint.x -= (static_cast<float>(children.size()) - 1) * inner_pad;
    }
    if (child_max_size_hint.y != UNDEFINED_SIZE) {
        child_max_size_hint.y -= 2 * border_size_();
        child_max_size_hint.y -= outer_pad.top + outer_pad.bottom;
    }

    if (grow == Grow::Vert || grow == Grow::None) {
        for (const auto &child: children)
            child->resize(child_max_size_hint);
    } else {
        // Check which elements won't grow horizontally
        auto growing_children = std::unordered_set<std::size_t>();
        for (const auto &[i, child]: children | std::views::enumerate) {
            if (child->grow == Grow::Vert || child->grow == Grow::None) {
                child->resize(UNDEFINED_SIZE_V);
                if (child_max_size_hint.x != UNDEFINED_SIZE)
                    child_max_size_hint.x -= child->size.x;
            } else {
                growing_children.insert(i);
            }
        }
        if (!growing_children.empty()) {
            if (child_max_size_hint.x != UNDEFINED_SIZE)
                child_max_size_hint.x /= static_cast<float>(growing_children.size());

            // Calculate the rest in the remaining space
            for (const std::size_t &i: growing_children)
                children[i]->resize({child_max_size_hint});
        }
    }

    if (size.x == UNDEFINED_SIZE) {
        size.x = std::ranges::fold_left(children, 0.0f, [](float sum, const auto &a) { return sum + a->size.x; });
        size.x += 2 * border_size_();
        size.x += outer_pad.left + outer_pad.right;
        size.x += (static_cast<float>(children.size() - 1)) * inner_pad;
    }

    if (size.y == UNDEFINED_SIZE) {
        size.y = std::ranges::max_element(
                         children, std::ranges::less{}, [](const auto &a) -> float { return a->size.y; })
                         ->get()
                         ->size.y;
        size.y += 2 * border_size_();
        size.y += outer_pad.top + outer_pad.bottom;

        child_max_size_hint.y = size.y - 2 * border_size_() - outer_pad.top - outer_pad.bottom;
        // One more pass with our newly calculated size
        // because something inside may be vertically growable
        for (const auto &child: children)
            child->resize(child_max_size_hint);
    }
}

void HStack::calc_bbox(glm::vec2 pos) {
    bbox = {pos.x, pos.y, pos.x + size.x, pos.y + size.y};

    glm::vec2 next_child_pos{pos.x + border_size_() + outer_pad.left, pos.y + border_size_() + outer_pad.top};
    for (const auto &child: children) {
        const auto center_offset = (size.y - 2 * border_size_() - outer_pad.top - outer_pad.bottom - child->size.y) / 2;
        child->calc_bbox({next_child_pos.x, next_child_pos.y + center_offset});

        next_child_pos.x += child->size.x;
        next_child_pos.x += inner_pad;
    }
}

glm::tvec2<Id> HStack::update(InputMgr &input, Id mouse_captured, Id keyboard_captured) {
    glm::tvec2<Id> new_capture = {NO_CAPTURE, NO_CAPTURE};
    for (const auto &child: children) {
        const auto child_capture = child->update(input, mouse_captured, keyboard_captured);
        if (child_capture.x != NO_CAPTURE)
            new_capture.x = child_capture.x;
        if (child_capture.y != NO_CAPTURE)
            new_capture.y = child_capture.y;
    }
    return new_capture;
}

void HStack::draw(G2d &g2d) const {
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

    for (const auto &child: children)
        child->draw(g2d);
}

float HStack::border_size_() const {
    if (border) {
        if (std::holds_alternative<PxBorder>(*border))
            return 1.0f;
        throw std::runtime_error("not yet implemented");
    }
    return 0.0f;
}

VSpacer::VSpacer() {
    grow = Grow::Vert;
}

void VSpacer::resize(const glm::vec2 &max_size_hint) {
    size.x = 0.0f;
    size.y = max_size_hint.y;
}

void VSpacer::calc_bbox(glm::vec2 pos) { /* invisible */ }

glm::tvec2<Id> VSpacer::update(InputMgr &input, Id mouse_captured, Id keyboard_captured) {
    return {NO_CAPTURE, NO_CAPTURE};
}

void VSpacer::draw(G2d &g2d) const { /* invisible */ }

HSpacer::HSpacer() {
    grow = Grow::Hori;
}

void HSpacer::resize(const glm::vec2 &max_size_hint) {
    size.x = max_size_hint.x;
    size.y = 0.0f;
}

void HSpacer::calc_bbox(glm::vec2 pos) { /* invisible */ }

glm::tvec2<Id> HSpacer::update(InputMgr &input, Id mouse_captured, Id keyboard_captured) {
    return {NO_CAPTURE, NO_CAPTURE};
}

void HSpacer::draw(G2d &g2d) const { /* invisible */ }
} // namespace mizu::gui
