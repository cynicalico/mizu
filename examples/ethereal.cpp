#include "SDL3/SDL.h"
#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif
#include "spdlog/spdlog.h"

int main(int, char *[]) {
#if !defined(NDEBUG)
    spdlog::set_level(spdlog::level::debug);
#endif

    SDL_SetAppMetadata("ethereal", "0.1", nullptr);
    SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, "ethereal");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SPDLOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
        return EXIT_FAILURE;
    }

    int sdl_version = SDL_GetVersion();
    SPDLOG_DEBUG("Initialized SDL v{}.{}.{}",
                 SDL_VERSIONNUM_MAJOR(sdl_version),
                 SDL_VERSIONNUM_MINOR(sdl_version),
                 SDL_VERSIONNUM_MICRO(sdl_version));

    SDL_Quit();

    return EXIT_SUCCESS;
}
