#ifndef MIZU_GUI_CONTROL_HPP
#define MIZU_GUI_CONTROL_HPP

#include <string>
#include "mizu/core/font.hpp"
#include "mizu/gui/node.hpp"

namespace mizu::gui {
class Button;

using ButtonOnClickFn = std::function<void(Button *)>;

struct ButtonParams {
    Grow grow{Grow::Both};
    Font *font;
    std::string text;
    Padding text_pad{0.0f};
    Rgba bg_color{rgba(0x00000000)};
    Rgba fg_color{rgb(0xffffff)};
    std::optional<Border> border{PxBorder{rgb(0xffffff)}};
    ButtonOnClickFn onclick{[](auto *) {}};
};

class Button final : public Node<ButtonParams> {
public:
    Font *font;
    std::string text;
    Padding text_pad;
    Rgba bg_color;
    Rgba fg_color;
    std::optional<Border> border;
    ButtonOnClickFn onclick;

    explicit Button(const ButtonParams &params);

    void resize(const glm::vec2 &max_size_hint) override;

    void calc_bbox(glm::vec2 pos) override;

    glm::tvec2<Id> update(InputMgr &input, Id mouse_captured, Id keyboard_captured) override;

    void draw(G2d &g2d) const override;

private:
    bool primed_{false};
    bool hovered_{false};

    float border_size_() const;
};
} // namespace mizu::gui

#endif // MIZU_GUI_CONTROL_HPP
