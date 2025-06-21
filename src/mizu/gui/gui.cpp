#include "mizu/gui/gui.hpp"
#include <unordered_set>

namespace mizu::gui {
constexpr auto UNDEFINED_SIZE = std::numeric_limits<float>::max();
constexpr auto UNDEFINED_SIZE_V = glm::vec2{UNDEFINED_SIZE, UNDEFINED_SIZE};

void Node::add_child(Node *child) {
    child->parent = this;
    children_.push_back(child);
}

void Node::set_grow(const Grow &grow) {
    grow_ = grow;
}

Grow Node::grow() const {
    return grow_;
}

glm::vec2 Node::size() const {
    return size_;
}

glm::vec2 VStack::calc_size(const glm::vec2 &max_size_hint) {
    switch (grow_) {
    case Grow::Hori: {
        size_ = {max_size_hint.x == UNDEFINED_SIZE ? 0 : max_size_hint.x, 0};
        glm::vec2 child_max_size_hint = {
                max_size_hint.x == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.x - 2 * BORDER_SIZE, UNDEFINED_SIZE};

        for (std::size_t i = 0; i < children_.size(); ++i) {
            children_[i]->calc_size(child_max_size_hint);
            size_.y += children_[i]->size().y;
        }
        size_.y += 2 * BORDER_SIZE;

        // Calculate our size now if we had no valid hint
        if (size_.x == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.x = std::max(size_.x, children_[i]->size().x);
            size_.x += 2 * BORDER_SIZE;

            child_max_size_hint.x = size_.x - 2 * BORDER_SIZE;
            // One more pass with our newly calculated size
            // because something inside may be horizontally growable
            for (std::size_t i = 0; i < children_.size(); ++i)
                children_[i]->calc_size(child_max_size_hint);
        }
    } break;
    case Grow::Vert: {
        size_ = {0, max_size_hint.y == UNDEFINED_SIZE ? 0 : max_size_hint.y};
        glm::vec2 child_max_size_hint = {
                UNDEFINED_SIZE, max_size_hint.y == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.y - 2 * BORDER_SIZE};

        // Check which elements won't grow vertically
        auto growing_children = std::unordered_set<std::size_t>();
        for (std::size_t i = 0; i < children_.size(); ++i) {
            if (children_[i]->grow() == Grow::Hori || children_[i]->grow() == Grow::None) {
                const auto child_size = children_[i]->calc_size(UNDEFINED_SIZE_V);
                size_.x = std::max(size_.x, child_size.x);
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
            for (const std::size_t &i: growing_children) {
                const auto child_size = children_[i]->calc_size(child_max_size_hint);
                size_.x = std::max(size_.x, child_size.x);
            }
        }
        size_.x += 2 * BORDER_SIZE;

        child_max_size_hint.x = size_.x - 2 * BORDER_SIZE;
        // One more pass with our newly calculated size
        // because something inside may be horizontally growable
        for (std::size_t i = 0; i < children_.size(); ++i)
            children_[i]->calc_size(child_max_size_hint);

        if (size_.y == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.y += children_[i]->size().y;
            size_.y += 2 * BORDER_SIZE;
        }
    } break;
    case Grow::Both: {
        size_ = {
                max_size_hint.x == UNDEFINED_SIZE ? 0 : max_size_hint.x,
                max_size_hint.y == UNDEFINED_SIZE ? 0 : max_size_hint.y};
        glm::vec2 child_max_size_hint = {
                max_size_hint.x == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.x - 2 * BORDER_SIZE,
                max_size_hint.y == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.y - 2 * BORDER_SIZE};

        // Check which elements won't grow vertically
        auto growing_children = std::unordered_set<std::size_t>();
        for (std::size_t i = 0; i < children_.size(); ++i) {
            if (children_[i]->grow() == Grow::Hori || children_[i]->grow() == Grow::None) {
                children_[i]->calc_size({child_max_size_hint.x, UNDEFINED_SIZE});
                const auto child_size = children_[i]->size();
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
                children_[i]->calc_size(child_max_size_hint);
        }

        // Calculate our size now if we had no valid hint
        if (size_.x == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.x = std::max(size_.x, children_[i]->size().x);
            size_.x += 2 * BORDER_SIZE;

            child_max_size_hint.x = size_.x - 2 * BORDER_SIZE;
            // One more pass with our newly calculated size
            // because something inside may be horizontally growable
            for (std::size_t i = 0; i < children_.size(); ++i)
                children_[i]->calc_size(child_max_size_hint);
        }
        if (size_.y == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.y += children_[i]->size().y;
            size_.y += 2 * BORDER_SIZE;
        }
    } break;
    case Grow::None: {
        size_ = {0, 0};
        glm::vec2 child_max_size_hint = {UNDEFINED_SIZE, UNDEFINED_SIZE};

        for (std::size_t i = 0; i < children_.size(); ++i) {
            children_[i]->calc_size(child_max_size_hint);
            const auto child_size = children_[i]->size();
            size_.x = std::max(size_.x, child_size.x);
            size_.y += child_size.y;
        }
        size_.x += 2 * BORDER_SIZE;
        size_.y += 2 * BORDER_SIZE;

        child_max_size_hint.x = size_.x - 2 * BORDER_SIZE;
        // One more pass with our newly calculated size
        // because something inside may be horizontally growable
        for (std::size_t i = 0; i < children_.size(); ++i)
            children_[i]->calc_size(child_max_size_hint);
    } break;
    }

    return size_;
}

void VStack::draw(G2d &g2d, glm::vec2 pos) {
    g2d.line(pos, {pos.x + size_.x - BORDER_SIZE, pos.y}, rgb(0xffffff));
    g2d.line(
            {pos.x + size_.x - BORDER_SIZE, pos.y},
            {pos.x + size_.x - 1, pos.y + size_.y - BORDER_SIZE},
            rgb(0xffffff));
    g2d.line(
            {pos.x + size_.x - BORDER_SIZE, pos.y + size_.y - 1},
            {pos.x, pos.y + size_.y - BORDER_SIZE},
            rgb(0xffffff));
    g2d.line({pos.x, pos.y + size_.y - BORDER_SIZE}, pos, rgb(0xffffff));

    glm::vec2 next_child_pos{pos.x + BORDER_SIZE, pos.y + BORDER_SIZE};
    for (std::size_t i = 0; i < children_.size(); ++i) {
        const auto child_size = children_[i]->size();
        children_[i]->draw(g2d, {next_child_pos.x + (size_.x - 2 * BORDER_SIZE - child_size.x) / 2, next_child_pos.y});
        next_child_pos.y += child_size.y;
    }
}

glm::vec2 HStack::calc_size(const glm::vec2 &max_size_hint) {
    switch (grow_) {
    case Grow::Hori: {
        size_ = {max_size_hint.x == UNDEFINED_SIZE ? 0 : max_size_hint.x, 0};
        glm::vec2 child_max_size_hint = {
                max_size_hint.x == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.x - 2 * BORDER_SIZE, UNDEFINED_SIZE};

        // Check which elements won't grow horizontally
        auto growing_children = std::unordered_set<std::size_t>();
        for (std::size_t i = 0; i < children_.size(); ++i) {
            if (children_[i]->grow() == Grow::Vert || children_[i]->grow() == Grow::None) {
                const auto child_size = children_[i]->calc_size(UNDEFINED_SIZE_V);
                size_.y = std::max(size_.y, child_size.y);
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
            for (const std::size_t &i: growing_children) {
                const auto child_size = children_[i]->calc_size(child_max_size_hint);
                size_.y = std::max(size_.y, child_size.y);
            }
        }
        size_.y += 2 * BORDER_SIZE;

        child_max_size_hint.y = size_.y - 2 * BORDER_SIZE;
        // One more pass with our newly calculated size
        // before something inside may be horizontally growable
        for (std::size_t i = 0; i < children_.size(); ++i)
            children_[i]->calc_size(child_max_size_hint);

        if (size_.x == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.x += children_[i]->size().x;
            size_.x += 2 * BORDER_SIZE;
        }
    } break;
    case Grow::Vert: {
        size_ = {0, max_size_hint.y == UNDEFINED_SIZE ? 0 : max_size_hint.y};
        glm::vec2 child_max_size_hint = {
                UNDEFINED_SIZE, max_size_hint.y == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.y - 2 * BORDER_SIZE};

        for (std::size_t i = 0; i < children_.size(); ++i) {
            const auto child_size = children_[i]->calc_size(child_max_size_hint);
            size_.x += child_size.x;
        }
        size_.x += 2 * BORDER_SIZE;

        if (size_.y == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.y = std::max(size_.y, children_[i]->size().y);
            size_.y += 2 * BORDER_SIZE;

            child_max_size_hint.y = size_.y - 2 * BORDER_SIZE;
            // One more pass with our newly calculated size
            // because something inside may be vertically growable
            for (std::size_t i = 0; i < children_.size(); ++i)
                children_[i]->calc_size(child_max_size_hint);
        }
    } break;
    case Grow::Both: {
        size_ = {
                max_size_hint.x == UNDEFINED_SIZE ? 0 : max_size_hint.x,
                max_size_hint.y == UNDEFINED_SIZE ? 0 : max_size_hint.y};
        glm::vec2 child_max_size_hint = {
                max_size_hint.x == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.x - 2,
                max_size_hint.y == UNDEFINED_SIZE ? UNDEFINED_SIZE : size_.y - 2};

        // Check which elements won't grow horizontally
        auto growing_children = std::unordered_set<std::size_t>();
        for (std::size_t i = 0; i < children_.size(); ++i) {
            if (children_[i]->grow() == Grow::Vert || children_[i]->grow() == Grow::None) {
                const auto child_size = children_[i]->calc_size({UNDEFINED_SIZE, child_max_size_hint.y});
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
                children_[i]->calc_size(child_max_size_hint);
        }

        if (size_.x == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.x += children_[i]->size().x;
            size_.x += 2 * BORDER_SIZE;
        }
        if (size_.y == 0) {
            for (std::size_t i = 0; i < children_.size(); ++i)
                size_.y = std::max(size_.y, children_[i]->size().y);
            size_.y += 2 * BORDER_SIZE;

            child_max_size_hint.y = size_.y - 2 * BORDER_SIZE;
            // One more pass with our newly calculated size
            // because something inside may be vertically growable
            for (std::size_t i = 0; i < children_.size(); ++i)
                children_[i]->calc_size(child_max_size_hint);
        }
    } break;
    case Grow::None: {
        size_ = {0, 0};
        glm::vec2 child_max_size_hint = {UNDEFINED_SIZE, UNDEFINED_SIZE};

        for (std::size_t i = 0; i < children_.size(); ++i) {
            const auto child_size = children_[i]->calc_size(child_max_size_hint);
            size_.x += child_size.x;
            size_.y = std::max(size_.y, child_size.y);
        }
        size_.x += 2 * BORDER_SIZE;
        size_.y += 2 * BORDER_SIZE;

        child_max_size_hint.y = size_.y - 2 * BORDER_SIZE;
        // One more pass with our newly calculated size
        // because something inside may be vertically growable
        for (std::size_t i = 0; i < children_.size(); ++i)
            children_[i]->calc_size(child_max_size_hint);
    } break;
    }

    return size_;
}

void HStack::draw(G2d &g2d, glm::vec2 pos) {
    g2d.line(pos, {pos.x + size_.x - BORDER_SIZE, pos.y}, rgb(0xffffff));
    g2d.line(
            {pos.x + size_.x - BORDER_SIZE, pos.y},
            {pos.x + size_.x - BORDER_SIZE, pos.y + size_.y - BORDER_SIZE},
            rgb(0xffffff));
    g2d.line(
            {pos.x + size_.x - BORDER_SIZE, pos.y + size_.y - BORDER_SIZE},
            {pos.x, pos.y + size_.y - BORDER_SIZE},
            rgb(0xffffff));
    g2d.line({pos.x, pos.y + size_.y - BORDER_SIZE}, pos, rgb(0xffffff));

    glm::vec2 next_child_pos{pos.x + BORDER_SIZE, pos.y + BORDER_SIZE};
    for (std::size_t i = 0; i < children_.size(); ++i) {
        const auto child_size = children_[i]->size();
        children_[i]->draw(g2d, {next_child_pos.x, next_child_pos.y + (size_.y - 2 * BORDER_SIZE - child_size.y) / 2});
        next_child_pos.x += child_size.x;
    }
}

Button::Button(CodePage437 *font, const std::string &text, float text_scale)
    : font_(font), text_(text), text_scale_(text_scale) {}

glm::vec2 Button::calc_size(const glm::vec2 &max_size_hint) {
    const auto text_size = font_->calculate_size(text_, text_scale_);

    switch (grow_) {
    case Grow::Hori: {
        size_.x = max_size_hint.x != UNDEFINED_SIZE ? max_size_hint.x : text_size.x + 2 + 2 * BORDER_SIZE;
        size_.y = text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    case Grow::Vert: {
        size_.x = text_size.x + 2 + 2 * BORDER_SIZE;
        size_.y = max_size_hint.y != UNDEFINED_SIZE ? max_size_hint.y : text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    case Grow::Both: {
        size_.x = max_size_hint.x != UNDEFINED_SIZE ? max_size_hint.x : text_size.x + 2 + 2 * BORDER_SIZE;
        size_.y = max_size_hint.y != UNDEFINED_SIZE ? max_size_hint.y : text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    case Grow::None: {
        size_.x = text_size.x + 2 + 2 * BORDER_SIZE;
        size_.y = text_size.y + 2 + 2 * BORDER_SIZE;
    } break;
    }

    return size_;
}

void Button::draw(G2d &g2d, glm::vec2 pos) {
    g2d.line(pos, {pos.x + size_.x - BORDER_SIZE, pos.y}, rgb(0xffff00));
    g2d.line(
            {pos.x + size_.x - BORDER_SIZE, pos.y},
            {pos.x + size_.x - BORDER_SIZE, pos.y + size_.y - BORDER_SIZE},
            rgb(0xffff00));
    g2d.line(
            {pos.x + size_.x - BORDER_SIZE, pos.y + size_.y - BORDER_SIZE},
            {pos.x, pos.y + size_.y - BORDER_SIZE},
            rgb(0xffff00));
    g2d.line({pos.x, pos.y + size_.y - BORDER_SIZE}, pos, rgb(0xffff00));

    const auto text_size = font_->calculate_size(text_, text_scale_);
    font_->draw(text_, {pos.x + (size_.x - text_size.x) / 2, pos.y + (size_.y - text_size.y) / 2}, text_scale_);
}
} // namespace mizu::gui
