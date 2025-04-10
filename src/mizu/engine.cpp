#include "mizu/engine.hpp"
#include <SDL3/SDL.h>
#include "mizu/log.hpp"

namespace mizu {
Engine::Engine(const std::string &window_title, const Size2d<int> window_size, const window_build_f &f)
    : gl(), running_(true) {
#if !defined(NDEBUG)
    spdlog::set_level(spdlog::level::debug);
#endif

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SPDLOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    int sdl_version = SDL_GetVersion();
    SPDLOG_DEBUG(
            "Initialized SDL v{}.{}.{}",
            SDL_VERSIONNUM_MAJOR(sdl_version),
            SDL_VERSIONNUM_MINOR(sdl_version),
            SDL_VERSIONNUM_MICRO(sdl_version)
    );

    auto builder = window_builder(window_title, window_size);
    auto window_result = f(builder.opengl().high_pixel_density());
    if (!window_result) {
        SPDLOG_ERROR("Failed to build window: {}", window_result.error());
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }
    window = std::make_unique<Window>(std::move(window_result.value()));

    auto glad_version = gladLoadGLContext(&gl, SDL_GL_GetProcAddress);
    if (!glad_version) {
        SPDLOG_ERROR("Failed to initialize OpenGL context");
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }
    SPDLOG_DEBUG("Initialized OpenGL v{}.{}", GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));
}

Engine::Engine(const std::string &window_title, const window_build_f &f) : Engine(window_title, Size2d(0, 0), f) {}

Engine::~Engine() {
    window.reset();

    SDL_Quit();
    SPDLOG_DEBUG("Quit SDL");
}

bool Engine::is_running_() const { return running_; }

void Engine::poll_events_() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            running_ = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.key) {
            case SDLK_ESCAPE:
                running_ = false;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}
} // namespace mizu
