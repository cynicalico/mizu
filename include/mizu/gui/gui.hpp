#ifndef MIZU_GUI_HPP
#define MIZU_GUI_HPP

#include <glm/vec2.hpp>
#include <vector>
#include "mizu/font/code_page_437.hpp"

namespace mizu::gui {
constexpr float BORDER_SIZE = 1.0f;

enum class Grow { Hori, Vert, Both, None };

struct PxBorder {
    Rgba color;
};

using Border = std::variant<PxBorder>;

struct Padding {
    float left, right, top, bottom;

    Padding(float left, float right, float top, float bottom)
        : left(left), right(right), top(top), bottom(bottom) {}

    Padding(float hori, float vert)
        : left(hori), right(hori), top(vert), bottom(vert) {}

    explicit Padding(float all)
        : left(all), right(all), top(all), bottom(all) {}
};

class Node {
public:
    glm::vec2 size{};
    std::vector<std::unique_ptr<Node>> children{};
    Grow grow{Grow::Both};

    Node *parent{nullptr};

    virtual ~Node() = default;

    virtual void calc_size(const glm::vec2 &max_size_hint) = 0;

    virtual void draw(G2d &g2d, glm::vec2 pos) const = 0;

    template<typename T, typename... Args>
        requires std::derived_from<T, Node>
    T *add_child(Args &&...args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        children.push_back(std::move(child));
        return static_cast<T *>(children.back().get());
    }
};

/***********
 * LAYOUTS *
 ***********/

class VStack final : public Node {
public:
    std::optional<Border> border{std::nullopt};
    Padding outer_pad{0.0f};
    float inner_pad{0.0f};

    VStack() = default;
    explicit VStack(Padding outer_pad, float inner_pad);

    void calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;

private:
    float border_size_() const;
};

class HStack final : public Node {
public:
    std::optional<Border> border{std::nullopt};
    Padding outer_pad{0.0f};
    float inner_pad{0.0f};

    HStack() = default;
    explicit HStack(Padding outer_pad, float inner_pad);

    void calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;

private:
    float border_size_() const;
};

/************
 * CONTROLS *
 ************/

class Button final : public Node {
public:
    CodePage437 *font;
    std::string text;
    float text_scale;

    Button(CodePage437 *font, const std::string &text, float text_scale = 1.0f);

    void calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;
};
} // namespace mizu::gui

#endif // MIZU_GUI_HPP
