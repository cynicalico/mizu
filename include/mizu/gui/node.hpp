#ifndef MIZU_GUI_NODE_HPP
#define MIZU_GUI_NODE_HPP

#include <glm/vec2.hpp>
#include <memory>
#include <vector>
#include "mizu/core/g2d.hpp"
#include "mizu/gui/types.hpp"

namespace mizu::gui {
template<typename P>
class Node;

class NodeI {
public:
    NodeI *parent{nullptr};
    glm::vec2 size{};
    glm::vec4 bbox{};
    std::vector<std::unique_ptr<NodeI>> children{};
    Grow grow{Grow::Both};

    virtual ~NodeI() = default;

    virtual void resize(const glm::vec2 &max_size_hint) = 0;

    virtual void calc_bbox(glm::vec2 pos) = 0;

    virtual void draw(G2d &g2d) const = 0;

    template<typename T>
        requires std::derived_from<T, Node<typename T::ParamType>>
    T *add_child(typename T::ParamType params);
};

template<typename P>
class Node : public NodeI {
public:
    using ParamType = P;
};

template<typename T>
    requires std::derived_from<T, Node<typename T::ParamType>>
T *NodeI::add_child(typename T::ParamType params) {
    auto child = std::make_unique<T>(params);
    child->parent = this;
    children.push_back(std::move(child));
    return static_cast<T *>(children.back().get());
}
} // namespace mizu::gui

#endif // MIZU_GUI_NODE_HPP
