#ifndef MIZU_GUI_GUI_HPP
#define MIZU_GUI_GUI_HPP

#include "mizu/gui/node.hpp"
#include "mizu/util/class_helpers.hpp"

namespace mizu::gui {
class Gui {
    friend class GuiBuilder;

public:
    glm::vec2 size() const;

    void calc_size(const glm::vec2 &max_size_hint);

    void draw(G2d &g2d, glm::vec2 pos) const;

private:
    std::unique_ptr<NodeI> root_;

    explicit Gui(std::unique_ptr<NodeI> root)
        : root_(std::move(root)) {}
};

class GuiBuilder {
public:
    GuiBuilder() = default;
    ~GuiBuilder() = default;

    NO_COPY(GuiBuilder)
    NO_MOVE(GuiBuilder)

    template<typename T>
        requires std::derived_from<T, Node<typename T::ParamType>>
    GuiBuilder &start(typename T::ParamType params);

    template<typename T>
        requires std::derived_from<T, Node<typename T::ParamType>>
    GuiBuilder &add(typename T::ParamType params);

    GuiBuilder &end();

    std::unique_ptr<Gui> build();

private:
    std::unique_ptr<NodeI> root_{nullptr};
    NodeI *current_{nullptr};
};

template<typename T>
    requires std::derived_from<T, Node<typename T::ParamType>>
GuiBuilder &GuiBuilder::start(typename T::ParamType params) {
    if (!root_) {
        root_ = std::make_unique<T>(params);
        current_ = root_.get();
    } else {
        current_ = current_->add_child<T>(params);
    }
    return *this;
}

template<typename T>
    requires std::derived_from<T, Node<typename T::ParamType>>
GuiBuilder &GuiBuilder::add(typename T::ParamType params) {
    current_->add_child<T>(params);
    return *this;
}
} // namespace mizu::gui

#endif // MIZU_GUI_GUI_HPP
