#ifndef MIZU_FONT_CODE_PAGE_437_HPP
#define MIZU_FONT_CODE_PAGE_437_HPP

#include "mizu/core/g2d.hpp"
#include "mizu/util/class_helpers.hpp"

namespace mizu {
class CodePage437 {
public:
    CodePage437(G2d &g2d, Texture &tex, glm::uvec2 char_size, unsigned int row_skip = 0);

    NO_COPY(CodePage437)
    NO_MOVE(CodePage437)

    unsigned int char_w() const;
    unsigned int char_h() const;

    glm::vec2 calculate_size(std::string_view text, float scale = 1.0f) const;

    void draw(std::string_view text, glm::vec2 pos, float scale, const Color &color = rgb(0xffffff)) const;
    void draw(std::string_view text, glm::vec2 pos, const Color &color = rgb(0xffffff)) const;

private:
    G2d &g2d_;
    Texture &tex_;
    glm::uvec2 char_size_;
    unsigned int row_skip_;
};
} // namespace mizu

#endif // MIZU_FONT_CODE_PAGE_437_HPP
