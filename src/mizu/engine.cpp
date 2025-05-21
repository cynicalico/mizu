#include "mizu/engine.hpp"
#include <SDL3/SDL.h>
#include <utility>
#include "gloo/sdl3/gl_attr.hpp"
#include "mizu/log.hpp"
#include "mizu/platform.hpp"

namespace mizu {
void gl_debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam
);

Engine::Engine(const std::string &window_title, Size2d<int> window_size, WindowBuildFunc f) : running_(true) {
#if !defined(NDEBUG)
    spdlog::set_level(spdlog::level::debug);
#endif
    log_platform();

    register_callbacks_();

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

    gloo::sdl3::GlAttr::set_context_version(gloo::GlContextVersion(4, 3));
    gloo::sdl3::GlAttr::set_context_profile(gloo::sdl3::GlProfile::Core);

#if !defined(NDEBUG)
    gloo::sdl3::GlAttr::set_context_flags().debug().set();
#endif

    auto builder = WindowBuilder(window_title, window_size);
    f(builder.opengl().high_pixel_density());

    auto window_result = builder.build(callbacks);
    if (!window_result) {
        SPDLOG_ERROR("Failed to build window: {}", window_result.error());
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }
    window = std::make_unique<Window>(std::move(window_result.value()));

    window->make_context_current();
    auto glad_version_opt = gl.load(SDL_GL_GetProcAddress);
    if (!glad_version_opt) {
        SPDLOG_ERROR("Failed to initialize OpenGL context");
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }

#if !defined(NDEBUG)
    gl.enable(gloo::GlCapability::DebugOutput);
    gl.enable(gloo::GlCapability::DebugOutputSync);
    gl.debug_message_callback(gl_debug_message_callback, nullptr);
#endif

    SPDLOG_DEBUG(
            "Loaded OpenGL v{}.{}, vendor: {}, renderer: {}",
            glad_version_opt->major,
            glad_version_opt->minor,
            reinterpret_cast<const char *>(gl.ctx.GetString(GL_VENDOR)),
            reinterpret_cast<const char *>(gl.ctx.GetString(GL_RENDERER))
    );

    input = std::make_unique<InputMgr>(callbacks);

    g2d = std::make_unique<G2d>(gl, callbacks);
}

Engine::Engine(const std::string &window_title, WindowBuildFunc f) : Engine(window_title, Size2d(0, 0), std::move(f)) {}

Engine::~Engine() {
    unregister_callbacks_();

    g2d.reset();
    input.reset();
    window.reset();

    SDL_Quit();
    SPDLOG_DEBUG("Quit SDL");
}

void Engine::shutdown() {
    running_ = false;
}

void Engine::poll_events_() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
            callbacks.pub<PEventKeyDown>(
                    event.key.timestamp, event.key.scancode, event.key.key, event.key.mod, event.key.repeat
            );
            break;
        case SDL_EVENT_KEY_UP:
            callbacks.pub<PEventKeyUp>(event.key.timestamp, event.key.scancode, event.key.key, event.key.mod);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            callbacks.pub<PEventMouseMotion>(
                    event.motion.timestamp, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel
            );
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            callbacks.pub<PEventMouseButtonDown>(
                    event.button.timestamp, event.button.button, event.button.clicks, event.button.x, event.button.y
            );
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            callbacks.pub<PEventMouseButtonUp>(
                    event.button.timestamp, event.button.button, event.button.x, event.button.y
            );
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            callbacks.pub<PEventMouseWheel>(
                    event.wheel.timestamp,
                    event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED,
                    event.wheel.x,
                    event.wheel.y,
                    event.wheel.mouse_x,
                    event.wheel.mouse_y
            );
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER: callbacks.pub<PEventMouseEnter>(event.window.timestamp); break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE: callbacks.pub<PEventMouseLeave>(event.window.timestamp); break;
        case SDL_EVENT_WINDOW_RESIZED:
            callbacks.pub<PEventWindowResized>(event.window.timestamp, event.window.data1, event.window.data2);
            break;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            callbacks.pub<PEventWindowPixelSizeChanged>(event.window.timestamp, event.window.data1, event.window.data2);
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED: callbacks.pub<PEventWindowFocusGained>(event.window.timestamp); break;
        case SDL_EVENT_WINDOW_FOCUS_LOST: callbacks.pub<PEventWindowFocusLost>(event.window.timestamp); break;
        case SDL_EVENT_QUIT: callbacks.pub<PEventQuit>(event.quit.timestamp); break;
        default: break;
        }
    }
}

void Engine::register_callbacks_() {
    callback_id_ = callbacks.reg();
    callbacks.sub<PEventQuit>(callback_id_, [&](const auto &) { running_ = false; });
}

void Engine::unregister_callbacks_() {
    callbacks.unsub<PEventQuit>(callback_id_);
    callback_id_ = 0;
}

void gl_debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam
) {
#define STRINGIFY(e)                                                                                                   \
    case e: return #e;

    const std::string source_str = std::invoke([source] {
        switch (source) {
            STRINGIFY(GL_DEBUG_SOURCE_API)
            STRINGIFY(GL_DEBUG_SOURCE_WINDOW_SYSTEM)
            STRINGIFY(GL_DEBUG_SOURCE_SHADER_COMPILER)
            STRINGIFY(GL_DEBUG_SOURCE_THIRD_PARTY)
            STRINGIFY(GL_DEBUG_SOURCE_APPLICATION)
            STRINGIFY(GL_DEBUG_SOURCE_OTHER)
        default: return "?";
        }
    });

    const std::string type_str = std::invoke([type] {
        switch (type) {
            STRINGIFY(GL_DEBUG_TYPE_ERROR)
            STRINGIFY(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
            STRINGIFY(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
            STRINGIFY(GL_DEBUG_TYPE_PORTABILITY)
            STRINGIFY(GL_DEBUG_TYPE_PERFORMANCE)
            STRINGIFY(GL_DEBUG_TYPE_MARKER)
            STRINGIFY(GL_DEBUG_TYPE_PUSH_GROUP)
            STRINGIFY(GL_DEBUG_TYPE_POP_GROUP)
            STRINGIFY(GL_DEBUG_TYPE_OTHER)
        default: return "?";
        }
    });

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        SPDLOG_ERROR("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        SPDLOG_WARN("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        SPDLOG_DEBUG("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        SPDLOG_TRACE("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    default: break; // won't happen
    }

#undef STRINGIFY
}
} // namespace mizu
