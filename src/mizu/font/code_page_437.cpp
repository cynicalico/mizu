#include "mizu/font/code_page_437.hpp"

namespace mizu {
CodePage437::CodePage437(G2d &g2d, Texture &tex, glm::uvec2 char_size, unsigned int row_skip)
    : g2d_(g2d), tex_(tex), char_size_(char_size), row_skip_(row_skip) {}

unsigned int CodePage437::char_w() const {
    return char_size_.x;
}

unsigned int CodePage437::char_h() const {
    return char_size_.y;
}

glm::vec2 CodePage437::calculate_size(std::string_view text, float scale) const {
    if (text.empty())
        return glm::vec2(0.0f);

    glm::vec2 size = {0.0f, 0.0f};

    float line_acc = 0.0f;
    for (const auto &c: text) {
        if (c == '\r')
            continue;

        if (c == '\n') {
            size.x = std::max(size.x, line_acc);
            size.y += char_size_.y * scale;
            line_acc = 0;
            continue;
        }

        // Catch the first real character
        if (size.y == 0.0f)
            size.y += char_size_.y * scale;
        line_acc += char_size_.x * scale;
    }
    size.x = std::max(size.x, line_acc);

    return size;
}

void CodePage437::draw(std::string_view text, glm::vec2 pos, float scale, const Color &color) const {
    glm::vec2 curr_pos = pos;
    for (const auto &c: text) {
        if (c == '\r')
            continue;

        if (c == '\n') {
            curr_pos.x = pos.x;
            curr_pos.y += char_size_.y * scale;
            continue;
        }

        g2d_.texture(
                tex_,
                curr_pos,
                {char_size_.x * scale, char_size_.y * scale},
                {char_size_.x * (c % 16), char_size_.y * (c / 16 - row_skip_), char_size_.x, char_size_.y},
                glm::vec3{0.0},
                color);

        curr_pos.x += char_size_.x * scale;
    }
}

void CodePage437::draw(std::string_view text, glm::vec2 pos, const Color &color) const {
    draw(text, pos, 1.0f, color);
}
} // namespace mizu
