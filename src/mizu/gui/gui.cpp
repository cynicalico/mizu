#include "mizu/gui/gui.hpp"
#include <algorithm>
#include <ranges>
#include <unordered_set>

namespace mizu::gui {
glm::vec2 Gui::size() const {
    return root_->size;
}

void Gui::calc_size(const glm::vec2 &max_size_hint) {
    root_->calc_size(max_size_hint);
}

void Gui::draw(G2d &g2d, glm::vec2 pos) const {
    root_->draw(g2d, pos);
}

GuiBuilder &GuiBuilder::end() {
    current_ = current_->parent;
    return *this;
}

std::unique_ptr<Gui> GuiBuilder::build() {
    current_ = nullptr;
    return std::unique_ptr<Gui>(new Gui(std::move(root_)));
}
} // namespace mizu::gui
