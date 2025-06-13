#ifndef MIZU_IO_HPP
#define MIZU_IO_HPP

#include <SDL3/SDL_surface.h>
#include <filesystem>
#include <optional>
#include <png.h>
#include <pngconf.h>
#include <string>
#include "mizu/class_helpers.hpp"

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
);
void end_png_read(FILE *fp, png_structpp png, png_infopp info);
} // namespace internal

struct PngData {
    png_uint_32 width, height, channels;
    unsigned char *bytes;

    PngData(png_uint_32 width, png_uint_32 height, png_uint_32 channels, std::size_t stride);
    ~PngData();

    NO_COPY(PngData)

    MOVE_CONSTRUCTOR(PngData);
    MOVE_ASSIGN_OP(PngData);
};

std::optional<PngData> read_image_data(const std::filesystem::path &path);

SDL_Surface *read_image_to_sdl_surface(const std::filesystem::path &path);

std::optional<std::string> read_file(const std::filesystem::path &path);
} // namespace mizu

#endif // MIZU_IO_HPP
