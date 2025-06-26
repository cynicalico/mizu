#define STB_RECT_PACK_IMPLEMENTATION
#include "../../../include/mizu/core/font.hpp"
#include <bitset>
#include <cstdio>
#include "../../../include/mizu/core/log.hpp"
#include "../../../include/mizu/util/platform.hpp"

namespace mizu {
std::once_flag Font::initialized_ft_;
FT_Library Font::ft_library_{nullptr};

Font::Font(G2d &g2d, const std::filesystem::path &path)
    : g2d_(g2d) {
    std::call_once(initialized_ft_, [&] {
        if (const auto err = FT_Init_FreeType(&ft_library_)) {
            MIZU_LOG_ERROR("Failed to initialize FreeType: {}", FT_Error_String(err));
        } else {
            FT_Int major, minor, patch;
            FT_Library_Version(ft_library_, &major, &minor, &patch);
            MIZU_LOG_DEBUG("Initialized FreeType v{}.{}.{}", major, minor, patch);
        }
    });

    if (!ft_library_)
        return;

    FILE *file;
#if defined(MIZU_PLATFORM_WINDOWS)
    if (const auto err = _wfopen_s(&file, path.c_str(), L"rb"); err != 0) {
        char buf[1024];
        strerror_s(buf, err);
        MIZU_LOG_ERROR("Failed to open font file '{}'", path, buf);
    }
#else
    // TODO: linux
#endif

    fseek(file, 0, SEEK_END);
    face_data_size_ = ftell(file);
    fseek(file, 0, SEEK_SET);

    face_data_ = static_cast<FT_Byte *>(malloc(sizeof(FT_Byte) * (face_data_size_ + 1)));
    fread(face_data_, face_data_size_, 1, file);
    fclose(file);

    if (const auto err = FT_New_Memory_Face(ft_library_, face_data_, face_data_size_, 0, &face_)) {
        MIZU_LOG_ERROR("Failed to create new FreeType face from '{}': {}", path, FT_Error_String(err));
        return;
    }
    MIZU_LOG_DEBUG("Created FreeType face for '{}'", path);

    atlas_ = g2d_.create_texture(ATLAS_SIZE);

    rp_nodes_ = new stbrp_node[ATLAS_SIZE.x];
    stbrp_init_target(&rp_ctx_, ATLAS_SIZE.x, ATLAS_SIZE.y, rp_nodes_, ATLAS_SIZE.x);

    glyphs_.emplace_back();
}

Font::~Font() {
    if (const auto err = FT_Done_Face(face_))
        MIZU_LOG_ERROR("Failed to close FreeType face: {}", FT_Error_String(err));
    free(face_data_);

    // // TODO: move this somewhere separate from loading faces, we can
    // //       only destroy one and then we'll have a problem
    // if (const auto err = FT_Done_FreeType(ft_library_))
    //     MIZU_LOG_ERROR("Failed to close FreeType: {}", FT_Error_String(err));
}

float Font::pen_offset() const {
    return pen_offset_;
}

float Font::line_height() const {
    return line_height_;
}

glm::vec2 Font::calc_size(std::string_view text) {
    if (text.empty())
        return glm::vec2(0.0f);

    check_populate_atlas_(text, 0);

    glm::vec2 size{0, line_height_};
    float curr_line_w = 0;

    for (const auto &ch: text) {
        if (ch == '\r')
            continue;

        if (ch == '\n') {
            size.y += line_height_;
            size.x = std::max(size.x, curr_line_w);
            curr_line_w = 0;
            continue;
        }

        const auto glyph_idx = FT_Get_Char_Index(face_, ch);
        const auto it = glyphs_[0].find(glyph_idx);
        if (it == glyphs_[0].end())
            continue; // can this happen?
        const auto &gi = it->second;

        curr_line_w += gi.advance.x;
    }

    size.x = std::max(size.x, curr_line_w);

    return size;
}

void Font::draw(const std::string_view text, glm::vec2 pos, const Color &color) {
    check_populate_atlas_(text, 0);

    glm::vec2 curr_pos = pos;
    // TODO: use a UTF-8 library instead of iterating naively over chars
    for (const auto &ch: text) {
        if (ch == '\r')
            continue;

        if (ch == '\n') {
            curr_pos.y += line_height_;
            curr_pos.x = pos.x;
            continue;
        }

        const auto glyph_idx = FT_Get_Char_Index(face_, ch);
        const auto it = glyphs_[0].find(glyph_idx);
        if (it == glyphs_[0].end())
            continue; // can this happen?
        const auto &gi = it->second;

        g2d_.texture(
                *atlas_,
                {curr_pos.x + gi.offset.x, curr_pos.y - gi.offset.y},
                {gi.pos.x, gi.pos.y, gi.size.x, gi.size.y},
                {0, 0, 0},
                color);

        curr_pos.x += gi.advance.x;
        curr_pos.y += gi.advance.y;
    }
}

void Font::check_populate_atlas_(const std::string_view text, int pt_size) {
    auto new_glyphs = std::vector<GlyphInfo>();
    auto new_glyph_bitmaps = std::vector<unsigned char *>();

    // TODO: use a UTF-8 library instead of iterating naively over chars
    for (const auto &ch: text) {
        const auto glyph_idx = FT_Get_Char_Index(face_, ch);
        if (glyphs_[pt_size].contains(glyph_idx))
            continue;
        glyphs_[pt_size].emplace(glyph_idx, GlyphInfo{});

        if (const auto err = FT_Load_Glyph(face_, glyph_idx, FT_LOAD_DEFAULT)) {
            MIZU_LOG_WARN("Failed to load glyph '{}': {}", ch, FT_Error_String(err));
            continue;
        }

        if (face_->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
            if (const auto err = FT_Render_Glyph(face_->glyph, FT_RENDER_MODE_NORMAL)) {
                MIZU_LOG_WARN("Failed to render glyph '{}': {}", ch, FT_Error_String(err));
                continue;
            }
        }

        switch (face_->glyph->bitmap.pixel_mode) {
        case FT_PIXEL_MODE_MONO: {
            if (pen_offset_ == 0)
                pen_offset_ = face_->glyph->bitmap_top;
            if (line_height_ == 0)
                line_height_ = face_->glyph->bitmap.rows;

            new_glyphs.emplace_back(
                    glyph_idx,
                    glm::vec2{0, 0},
                    glm::vec2{face_->glyph->bitmap.width, face_->glyph->bitmap.rows},
                    glm::vec2{face_->glyph->bitmap_left, face_->glyph->bitmap_top},
                    glm::vec2{face_->glyph->advance.x >> 6, face_->glyph->advance.y >> 6});

            auto *rgba_bitmap = new unsigned char[face_->glyph->bitmap.width * face_->glyph->bitmap.rows * 4];
            for (std::size_t y = 0; y < face_->glyph->bitmap.rows; y++) {
                std::size_t bitmap_x = 0;

                for (std::size_t yo = 0; yo < face_->glyph->bitmap.pitch; yo++) {
                    const auto bitset =
                            std::bitset<8>(face_->glyph->bitmap.buffer[(y * face_->glyph->bitmap.pitch) + yo]);

                    for (std::size_t i = 0; i < bitset.size() && bitmap_x < face_->glyph->bitmap.width;
                         ++i, ++bitmap_x) {
                        const auto v = static_cast<unsigned char>(bitset[bitset.size() - 1 - i] == 1 ? 255 : 0);
                        rgba_bitmap[face_->glyph->bitmap.width * 4 * y + bitmap_x * 4] = v;
                        rgba_bitmap[face_->glyph->bitmap.width * 4 * y + bitmap_x * 4 + 1] = v;
                        rgba_bitmap[face_->glyph->bitmap.width * 4 * y + bitmap_x * 4 + 2] = v;
                        rgba_bitmap[face_->glyph->bitmap.width * 4 * y + bitmap_x * 4 + 3] = v;
                    }
                }
            }
            new_glyph_bitmaps.emplace_back(rgba_bitmap);
        } break;
        // case FT_PIXEL_MODE_GRAY: {
        //     new_glyphs.emplace_back(
        //             glyph_idx,
        //             glm::vec2{0, 0},
        //             glm::vec2{face_->glyph->bitmap.width, face_->glyph->bitmap.rows},
        //             glm::vec2{face_->glyph->bitmap_left, face_->glyph->bitmap_top},
        //             glm::vec2{face_->glyph->advance.x >> 6, face_->glyph->advance.y >> 6});
        //
        //     auto *rgba_bitmap = new unsigned char[face_->glyph->bitmap.width * face_->glyph->bitmap.rows * 4];
        //     for (std::size_t y = 0; y < face_->glyph->bitmap.rows; y++) {
        //         for (std::size_t x = 0; x < face_->glyph->bitmap.width; x++) {
        //             const auto v = face_->glyph->bitmap.buffer[face_->glyph->bitmap.width * y + x];
        //             rgba_bitmap[face_->glyph->bitmap.width * 4 * y + x * 4] = 255;
        //             rgba_bitmap[face_->glyph->bitmap.width * 4 * y + x * 4 + 1] = 255;
        //             rgba_bitmap[face_->glyph->bitmap.width * 4 * y + x * 4 + 2] = 255;
        //             rgba_bitmap[face_->glyph->bitmap.width * 4 * y + x * 4 + 3] = v;
        //         }
        //     }
        //     new_glyph_bitmaps.emplace_back(rgba_bitmap);
        // } break;
        default: MIZU_LOG_ERROR("Pixel mode {} not yet implemented", face_->glyph->bitmap.pixel_mode);
        }
    }

    // No new glyphs to pack
    if (new_glyphs.empty())
        return;

    std::vector<stbrp_rect> new_rects;
    new_rects.reserve(new_glyphs.size());
    for (std::size_t i = 0; i < new_glyphs.size(); ++i)
        new_rects.emplace_back(i, new_glyphs[i].size.x, new_glyphs[i].size.y, 0, 0, 0);

    bool all_rects_packed = stbrp_pack_rects(&rp_ctx_, &new_rects[0], new_rects.size());

    std::size_t rects_packed = 0;
    for (std::size_t i = 0; i < new_rects.size(); ++i) {
        if (!new_rects[i].was_packed)
            continue;
        rects_packed++;

        const auto new_glyph_idx = new_rects[i].id;
        new_glyphs[new_glyph_idx].pos = {new_rects[i].x, new_rects[i].y};
        atlas_->write_subimage(
                new_glyphs[new_glyph_idx].pos, new_glyphs[new_glyph_idx].size, new_glyph_bitmaps[new_glyph_idx]);
        glyphs_[pt_size][new_glyphs[new_glyph_idx].idx] = new_glyphs[new_glyph_idx];
    }

    if (rects_packed > 0)
        MIZU_LOG_TRACE("Packed {} rects", rects_packed);
    if (!all_rects_packed)
        MIZU_LOG_WARN("Failed to pack {}", new_rects.size() - rects_packed);

    for (const auto &bmp: new_glyph_bitmaps)
        delete[] bmp;
}
} // namespace mizu
