#ifndef MIZU_GUI_HPP
#define MIZU_GUI_HPP

#include <glm/vec2.hpp>
#include <vector>
#include "mizu/font/code_page_437.hpp"

namespace mizu::gui {
constexpr float BORDER_SIZE = 1.0f;

enum class Grow { Hori, Vert, Both, None };

struct Padding {
    float left, right, top, bottom;

    Padding(float left, float right, float top, float bottom)
        : left(left), right(right), top(top), bottom(bottom) {}

    explicit Padding(float all)
        : left(all), right(all), top(all), bottom(all) {}
};

class Node {
public:
    glm::vec2 size{};
    std::vector<Node *> children{};
    Grow grow{Grow::Both};

    Node *parent{nullptr};

    virtual ~Node() = default;

    virtual glm::vec2 calc_size(const glm::vec2 &max_size_hint) = 0;

    virtual void draw(G2d &g2d, glm::vec2 pos) const = 0;
};

/***********
 * LAYOUTS *
 ***********/

class VStack : public Node {
public:
    Padding outer_pad{0.0f};
    float inner_pad{0.0f};

    VStack() = default;
    explicit VStack(Padding outer_pad, float inner_pad);

    glm::vec2 calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;
};

class HStack : public Node {
public:
    Padding outer_pad{0.0f};
    float inner_pad{0.0f};

    HStack() = default;
    explicit HStack(Padding outer_pad, float inner_pad);

    glm::vec2 calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;
};

/************
 * CONTROLS *
 ************/

class Button : public Node {
public:
    Button(CodePage437 *font, const std::string &text, float text_scale = 1.0f);

    glm::vec2 calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;

private:
    CodePage437 *font_;
    std::string text_;
    float text_scale_;
};
} // namespace mizu::gui

#endif // MIZU_GUI_HPP
