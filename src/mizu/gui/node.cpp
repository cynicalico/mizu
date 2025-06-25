#include "mizu/gui/node.hpp"

namespace mizu::gui {
Id next_id() {
    static Id curr_id = 0;
    return ++curr_id;
}

NodeI::NodeI()
    : id(next_id()) {}

MOVE_CONSTRUCTOR_IMPL(NodeI)
    : id(other.id),
      parent(other.parent),
      size(other.size),
      bbox(other.bbox),
      children(std::move(other.children)),
      grow(other.grow) {
    other.id = 0;
    other.parent = nullptr;
    other.size = {0, 0};
    other.bbox = {0, 0, 0, 0};
    other.children.clear();
    other.grow = Grow::Both;
}

MOVE_ASSIGN_OP_IMPL(NodeI) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        parent = other.parent;
        other.parent = nullptr;

        size = other.size;
        other.size = {0, 0};

        bbox = other.bbox;
        other.bbox = {0, 0, 0, 0};

        children = std::move(other.children);
        other.children.clear();

        grow = other.grow;
        other.grow = Grow::Both;
    }
    return *this;
}
} // namespace mizu::gui
