#include "mizu/io.hpp"
#include <fstream>
#include "mizu/log.hpp"
#include "mizu/platform.hpp"

namespace mizu {
namespace internal {
bool prepare_png_read(
        const std::filesystem::path &path,
        FILE *fp,
        png_structp &png,
        png_infop &info,
        std::size_t &stride,
        png_uint_32 &width,
        png_uint_32 &height,
        png_uint_32 &channels
) {
#if defined(MIZU_PLATFORM_WINDOWS)
    if (fopen_s(&fp, path.string().c_str(), "rb") != 0) {
#else
    fp = fopen(path.string().c_str(), "rb");
    if (!fp) {
#endif
        MIZU_LOG_ERROR("Failed to open file: '{}'", path);
        return false;
    }

    unsigned char header[8];
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8) != 0) {
        MIZU_LOG_ERROR("Header signature is not correct");
        end_png_read(fp, nullptr, nullptr);
        return false;
    }

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        MIZU_LOG_ERROR("Failed to create png read struct");
        end_png_read(fp, nullptr, nullptr);
        return false;
    }

    info = png_create_info_struct(png);
    if (!info) {
        MIZU_LOG_ERROR("Failed to create png info struct");
        end_png_read(fp, &png, nullptr);
        return false;
    }

    if (setjmp(png_jmpbuf(png))) {
        end_png_read(fp, &png, &info);
        return false;
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8); // we already read the 8 signature bytes
    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    auto color_type = png_get_color_type(png, info);
    auto bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) {
#if PNG_LIBPNG_VER >= 10'504
        png_set_scale_16(png);
#else
        png_set_strip_16(png);
#endif
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png, 0xffff, PNG_FILLER_AFTER);
        png_set_add_alpha(png, 0xff, PNG_FILLER_AFTER);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    stride = png_get_rowbytes(png, info);
    channels = png_get_channels(png, info);
    assert(channels == 4);

    if (height > PNG_SIZE_MAX / stride) {
        MIZU_LOG_ERROR("Image buffer would be too large");
        end_png_read(fp, &png, &info);
        return false;
    }

    return true;
}

void end_png_read(FILE *fp, png_structpp png, png_infopp info) {
    png_destroy_read_struct(png, info, nullptr);
    if (fp)
        fclose(fp);
}
} // namespace internal

PngData::PngData(png_uint_32 width, png_uint_32 height, png_uint_32 channels, std::size_t stride)
    : width(width), height(height), channels(channels) {
    assert(stride == width * channels);
    bytes = new unsigned char[stride * height];
}

PngData::~PngData() {
    delete[] bytes;
}

MOVE_CONSTRUCTOR_IMPL(PngData)
    : width(other.width), height(other.height), channels(other.channels), bytes(other.bytes) {
    other.width = 0;
    other.height = 0;
    other.channels = 0;
    other.bytes = nullptr;
}

MOVE_ASSIGN_OP_IMPL(PngData) {
    if (this != &other) {
        width = other.width;
        other.width = 0;

        height = other.height;
        other.height = 0;

        channels = other.channels;
        other.channels = 0;

        bytes = other.bytes;
        other.bytes = nullptr;
    }
    return *this;
}

std::optional<PngData> read_image_data(const std::filesystem::path &path) {
    FILE *fp = nullptr;
    png_structp png;
    png_infop info;
    std::size_t stride;
    png_uint_32 width;
    png_uint_32 height;
    png_uint_32 channels;
    if (!internal::prepare_png_read(path, fp, png, info, stride, width, height, channels))
        return std::nullopt;

    auto data = PngData(width, height, channels, stride);

    std::vector<png_bytep> row_pointers(height);
    for (std::size_t y = 0; y < height; y++)
        row_pointers[y] = data.bytes + y * stride;
    png_read_image(png, row_pointers.data());
    png_read_end(png, nullptr);

    internal::end_png_read(fp, &png, &info);

    return data;
}

SDL_Surface *read_image_to_sdl_surface(const std::filesystem::path &path) {
    FILE *fp = nullptr;
    png_structp png;
    png_infop info;
    std::size_t stride;
    png_uint_32 width;
    png_uint_32 height;
    png_uint_32 channels;
    if (!internal::prepare_png_read(path, fp, png, info, stride, width, height, channels))
        return nullptr;

    auto surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    if (!surface) {
        MIZU_LOG_ERROR("Failed to create SDL surface: {}", SDL_GetError());
        internal::end_png_read(fp, &png, &info);
        return nullptr;
    }

    std::vector<png_bytep> row_pointers(height);
    for (std::size_t y = 0; y < height; y++)
        row_pointers[y] = static_cast<png_bytep>(surface->pixels) + y * stride;
    png_read_image(png, row_pointers.data());
    png_read_end(png, nullptr);

    internal::end_png_read(fp, &png, &info);

    return surface;
}

std::optional<std::string> read_file(const std::filesystem::path &path) {
    const auto size = file_size(path);
    std::string content(size, '\0');

    std::ifstream in(path);
    if (!in.is_open()) {
        MIZU_LOG_ERROR("Failed to open file: '{}'", path);
        return std::nullopt;
    }
    in.read(&content[0], size);

    return content;
}
} // namespace mizu
