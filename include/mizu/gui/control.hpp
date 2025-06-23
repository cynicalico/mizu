#ifndef MIZU_GUI_CONTROL_HPP
#define MIZU_GUI_CONTROL_HPP

#include <string>
#include "mizu/font/code_page_437.hpp"
#include "mizu/gui/node.hpp"

namespace mizu::gui {
struct ButtonParams {
    CodePage437 *font;
    std::string text;
    float text_scale;
};

class Button final : public Node<ButtonParams> {
public:
    CodePage437 *font;
    std::string text;
    float text_scale;

    explicit Button(ButtonParams params);

    void calc_size(const glm::vec2 &max_size_hint) override;

    void draw(G2d &g2d, glm::vec2 pos) const override;
};
} // namespace mizu::gui

#endif // MIZU_GUI_CONTROL_HPP
