#ifndef MIZU_GUI_HPP
#define MIZU_GUI_HPP

#include <glm/vec2.hpp>
#include <vector>
#include "mizu/font/code_page_437.hpp"

namespace mizu::gui {
constexpr float BORDER_SIZE = 1.0f;

enum class Grow { Hori, Vert, Both, None };

class Node {
public:
    Node *parent{nullptr};

    virtual ~Node() = default;

    virtual void add_child(Node *child);

    void set_grow(const Grow &grow);
    Grow grow() const;

    virtual glm::vec2 calc_size(const glm::vec2 &max_size_hint) = 0;
    glm::vec2 size() const;

    virtual void draw(G2d &g2d, glm::vec2 pos) = 0;

protected:
    glm::vec2 size_{};
    std::vector<Node *> children_{};
    Grow grow_{Grow::Both};
};

/***********
 * LAYOUTS *
 ***********/

class VStack : public Node {
public:
    glm::vec2 calc_size(const glm::vec2 &max_allowed_size) override;

    void draw(G2d &g2d, glm::vec2 pos) override;
};

class HStack : public Node {
public:
    glm::vec2 calc_size(const glm::vec2 &max_allowed_size) override;

    void draw(G2d &g2d, glm::vec2 pos) override;
};

/************
 * CONTROLS *
 ************/

class Button : public Node {
public:
    Button(CodePage437 *font, const std::string &text, float text_scale = 1.0f);

    glm::vec2 calc_size(const glm::vec2 &max_allowed_size) override;

    void draw(G2d &g2d, glm::vec2 pos) override;

private:
    CodePage437 *font_;
    std::string text_;
    float text_scale_;
};
} // namespace mizu::gui

#endif // MIZU_GUI_HPP
