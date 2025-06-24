#include "mizu/gui/gui.hpp"

namespace mizu::gui {
Gui::Gui(std::unique_ptr<NodeI> root)
    : root_(std::move(root)) {}

glm::vec2 Gui::size() const {
    return root_->size;
}

void Gui::update(InputMgr &input) {
    captured_ = root_->update(input, captured_);
}

void Gui::resize(const glm::vec2 &max_size_hint, const glm::vec2 &pos) {
    root_->resize(max_size_hint);
    root_->calc_bbox(pos);
}

void Gui::draw(G2d &g2d) const {
    root_->draw(g2d);
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
