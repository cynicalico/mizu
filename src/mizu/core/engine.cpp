#include "mizu/core/engine.hpp"
#include <SDL3/SDL.h>
#include <utility>
#include "gloo/sdl3/attr.hpp"
#include "mizu/core/log.hpp"
#include "mizu/util/platform.hpp"

#if defined(MIZU_PLATFORM_WINDOWS)
#include "windows.h"
#endif

namespace mizu {
void gl_debug_message_callback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam);

Engine::Engine(const std::string &window_title, glm::ivec2 window_size, WindowBuildFunc f)
    : running_(true), show_fps_(false) {
    // Log levels are controlled through MIZU_SPDLOG_LEVEL, but we don't know what the user
    // has set, so just assume trace logging to catch everything
    spdlog::set_level(spdlog::level::trace);
#if defined(MIZU_PLATFORM_WINDOWS)
    SetConsoleOutputCP(CP_UTF8);
#endif
    log_platform();

    register_callbacks_();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        MIZU_LOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }

    int sdl_version = SDL_GetVersion();
    MIZU_LOG_DEBUG(
            "Initialized SDL v{}.{}.{}",
            SDL_VERSIONNUM_MAJOR(sdl_version),
            SDL_VERSIONNUM_MINOR(sdl_version),
            SDL_VERSIONNUM_MICRO(sdl_version));

    gloo::sdl3::Attr::set_context_version(gloo::ContextVersion(4, 5));
    gloo::sdl3::Attr::set_context_profile(gloo::sdl3::Profile::Core);

#if !defined(NDEBUG)
    gloo::sdl3::Attr::set_context_flags().debug().set();
#endif

    auto builder = WindowBuilder(window_title, window_size);
    f(builder.opengl().high_pixel_density());

    auto window_result = builder.build(callbacks);
    if (!window_result) {
        MIZU_LOG_ERROR("Failed to build window: {}", window_result.error());
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }
    window = std::make_unique<Window>(std::move(window_result.value()));

    window->make_context_current();
    auto glad_version_opt = gl.load(SDL_GL_GetProcAddress);
    if (!glad_version_opt) {
        MIZU_LOG_ERROR("Failed to initialize OpenGL context");
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }

#if !defined(NDEBUG)
    gl.enable(gloo::Capability::DebugOutput);
    gl.enable(gloo::Capability::DebugOutputSync);
    gl.debug_message_callback(gl_debug_message_callback, nullptr);
#endif

    MIZU_LOG_DEBUG(
            "Loaded OpenGL v{}.{}, vendor: {}, renderer: {}",
            glad_version_opt->major,
            glad_version_opt->minor,
            reinterpret_cast<const char *>(gl.ctx.GetString(GL_VENDOR)),
            reinterpret_cast<const char *>(gl.ctx.GetString(GL_RENDERER)));

    input = std::make_unique<InputMgr>(callbacks, window.get());

    g2d = std::make_unique<G2d>(callbacks, gl, window.get());
    g2d->set_vsync(1);

    dear = std::make_unique<Dear>(callbacks, window.get());

#if defined(MIZU_FEATURE_AUDIO)
    audio = std::make_unique<AudioMgr>(callbacks);
#endif

    frame_counter = FrameCounter();
}

Engine::Engine(const std::string &window_title, WindowBuildFunc f)
    : Engine(window_title, glm::ivec2(0), std::move(f)) {}

Engine::~Engine() {
    unregister_callbacks_();


#if defined(MIZU_FEATURE_AUDIO)
    audio.reset();
#endif
    dear.reset();
    g2d.reset();
    input.reset();
    window.reset();

    SDL_Quit();
    MIZU_LOG_DEBUG("Quit SDL");
}

void Engine::shutdown() {
    running_ = false;
}

bool Engine::show_fps() const {
    return show_fps_;
}

void Engine::set_show_fps(bool v) {
    show_fps_ = v;
}

void Engine::poll_events_() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        dear->process_sdl3_event(event);

        switch (event.type) {
        case SDL_EVENT_KEY_DOWN:
            if (!dear->ignore_keyboard_inputs()) {
                callbacks.pub<PEventKeyDown>(
                        event.key.timestamp, event.key.scancode, event.key.key, event.key.mod, event.key.repeat);
            }
            break;
        case SDL_EVENT_KEY_UP:
            if (!dear->ignore_keyboard_inputs())
                callbacks.pub<PEventKeyUp>(event.key.timestamp, event.key.scancode, event.key.key, event.key.mod);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            if (!dear->ignore_mouse_inputs()) {
                callbacks.pub<PEventMouseMotion>(
                        event.motion.timestamp, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (!dear->ignore_mouse_inputs()) {
                callbacks.pub<PEventMouseButtonDown>(
                        event.button.timestamp,
                        event.button.button,
                        event.button.clicks,
                        event.button.x,
                        event.button.y);
            }
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (!dear->ignore_mouse_inputs()) {
                callbacks.pub<PEventMouseButtonUp>(
                        event.button.timestamp, event.button.button, event.button.x, event.button.y);
            }
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            if (!dear->ignore_mouse_inputs()) {
                callbacks.pub<PEventMouseWheel>(
                        event.wheel.timestamp,
                        event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED,
                        event.wheel.x,
                        event.wheel.y,
                        event.wheel.mouse_x,
                        event.wheel.mouse_y);
            }
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            if (!dear->ignore_mouse_inputs())
                callbacks.pub<PEventMouseEnter>(event.window.timestamp);
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            if (!dear->ignore_mouse_inputs())
                callbacks.pub<PEventMouseLeave>(event.window.timestamp);
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            callbacks.pub<PEventWindowResized>(event.window.timestamp, event.window.data1, event.window.data2);
            break;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            callbacks.pub<PEventWindowPixelSizeChanged>(event.window.timestamp, event.window.data1, event.window.data2);
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED: callbacks.pub<PEventWindowFocusGained>(event.window.timestamp); break;
        case SDL_EVENT_WINDOW_FOCUS_LOST: callbacks.pub<PEventWindowFocusLost>(event.window.timestamp); break;
        case SDL_EVENT_QUIT: callbacks.pub<PEventQuit>(event.quit.timestamp); break;
        case SDL_EVENT_TEXT_INPUT:
            if (!dear->ignore_keyboard_inputs())
                callbacks.pub<PEventTextInput>(event.text.timestamp, event.text.text);
            break;
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
        const void *userParam) {
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
        MIZU_LOG_ERROR("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        MIZU_LOG_WARN("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        MIZU_LOG_DEBUG("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id, message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        // MIZU_LOG_TRACE("OpenGL: source={} type={} id={} msg={}", source_str.substr(9), type_str.substr(9), id,
        // message);
        break;
    default: break; // won't happen
    }

#undef STRINGIFY
}
} // namespace mizu
