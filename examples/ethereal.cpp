#include <SDL3/SDL.h>
#include <glad/gl.h>
#include "mizu/log.hpp"
#include "mizu/window.hpp"

int main(int, char *[]) {
#if !defined(NDEBUG)
    spdlog::set_level(spdlog::level::debug);
#endif

    auto build_result = mizu::window_builder("ethereal").opengl().display(1).fullscreen().build();
    if (!build_result.has_value()) {
        SPDLOG_ERROR("Failed to build window: {}", build_result.error());
        return EXIT_FAILURE;
    }
    auto window = std::move(build_result.value());

    GladGLContext gl;
    auto glad_version = gladLoadGLContext(&gl, SDL_GL_GetProcAddress);
    if (!glad_version) {
        SPDLOG_ERROR("Failed to initialize OpenGL context");
        return EXIT_FAILURE;
    }

    SPDLOG_DEBUG("Initialized OpenGL v{}.{}", GLAD_VERSION_MAJOR(glad_version), GLAD_VERSION_MINOR(glad_version));

    bool running = true;
    do {
        gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.swap();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        running = false;
                    }
                    break;
                default:
                    break;
            }
        }
    } while (running);

    return EXIT_SUCCESS;
}
