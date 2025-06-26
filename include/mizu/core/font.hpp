#ifndef MIZU_FONT_TTF_HPP
#define MIZU_FONT_TTF_HPP

#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <mutex>
#include "g2d.hpp"
#include "mizu/util/class_helpers.hpp"
#include "stb_rect_pack.h"
#include "texture.hpp"

namespace mizu {
constexpr glm::ivec2 ATLAS_SIZE{512, 512};

class Font {
public:
    explicit Font(G2d &g2d, const std::filesystem::path &path);

    ~Font();

    NO_COPY(Font)
    NO_MOVE(Font)

    float pen_offset() const;
    float line_height() const;

    glm::vec2 calc_size(std::string_view text);

    void draw(std::string_view text, glm::vec2 pos, const Color &color = rgb(0xffffff));

private:
    G2d &g2d_;
    float pen_offset_{0};
    float line_height_{0};

    FT_Byte *face_data_{nullptr};
    std::size_t face_data_size_{0};
    FT_Face face_{nullptr};

    struct GlyphInfo {
        std::uint32_t idx;
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec2 offset;
        glm::vec2 advance;
    };
    std::unique_ptr<Texture> atlas_{nullptr};
    std::vector<std::unordered_map<std::uint32_t, GlyphInfo>> glyphs_{};

    stbrp_context rp_ctx_;
    stbrp_node *rp_nodes_{nullptr};
    void check_populate_atlas_(std::string_view text, int pt_size);

    static std::once_flag initialized_ft_;
    static FT_Library ft_library_;
};
} // namespace mizu

#endif // MIZU_FONT_TTF_HPP
