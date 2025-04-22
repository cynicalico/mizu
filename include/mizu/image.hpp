#ifndef MIZU_IMAGE_HPP
#define MIZU_IMAGE_HPP

#include <SDL3/SDL_surface.h>
#include <filesystem>

namespace mizu {
SDL_Surface *read_image_to_sdl_surface(const std::filesystem::path &path);
} // namespace mizu

#endif // MIZU_IMAGE_HPP
