#include "mizu/image.hpp"
#include <png.h>
#include "mizu/log.hpp"
#include "mizu/platform.hpp"

namespace mizu {
SDL_Surface *read_image_to_sdl_surface(const std::filesystem::path &path) {
#if defined(MIZU_PLATFORM_WINDOWS)
    FILE *fp;
    if (fopen_s(&fp, path.string().c_str(), "rb") != 0) {
#else
    FILE *fp = fopen(path.string().c_str(), "rb");
    if (!fp) {
#endif
        SPDLOG_ERROR("Failed to open file: '{}'", path.string());
        return nullptr;
    }

    unsigned char header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8) != 0) {
        SPDLOG_ERROR("Header signature is not correct");
        fclose(fp);
        return nullptr;
    }

    auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        SPDLOG_ERROR("Failed to create png read struct");
        return nullptr;
    }

    auto info = png_create_info_struct(png);
    if (!info) {
        SPDLOG_ERROR("Failed to create png info struct");
        png_destroy_read_struct(&png, nullptr, nullptr);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(fp);
        return nullptr;
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8); // we already read the 8 signature bytes
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    auto color_type = png_get_color_type(png, info);
    auto bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) {
#if PNG_LIBPNG_VER >= 10'504
        png_set_scale_16(png);
#else
        png_set_strip_16(png);
#endif
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xffff, PNG_FILLER_AFTER);
        png_set_add_alpha(png, 0xff, PNG_FILLER_AFTER);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    std::size_t stride = png_get_rowbytes(png, info);

    if (height > PNG_SIZE_MAX / stride) {
        SPDLOG_ERROR("Image buffer would be too large");
        png_destroy_read_struct(&png, &info, nullptr);
        return nullptr;
    }

    auto surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        SPDLOG_ERROR("Failed to create SDL surface: {}", SDL_GetError());
        png_destroy_read_struct(&png, &info, nullptr);
        return nullptr;
    }

    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; y++)
        row_pointers[y] = static_cast<png_bytep>(surface->pixels) + y * stride;
    png_read_image(png, row_pointers.data());

    fclose(fp);

    png_destroy_read_struct(&png, &info, nullptr);

    return surface;
}
} // namespace mizu
