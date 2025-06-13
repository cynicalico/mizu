#include "mizu/window.hpp"
#include <SDL3/SDL.h>
#include <glm/ext/matrix_clip_space.hpp>
#include "mizu/io.hpp"
#include "mizu/log.hpp"
#include "mizu/payloads.hpp"

namespace mizu {
Window::~Window() {
    unregister_callbacks_();

    if (gl_context_) {
        if (!SDL_GL_DestroyContext(gl_context_))
            MIZU_LOG_ERROR("Failed to destroy GL context: {}", SDL_GetError());
        else
            MIZU_LOG_DEBUG("Destroyed GL context");
        gl_context_ = nullptr;
    }

    if (sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
        MIZU_LOG_DEBUG("Destroyed SDL window");
        sdl_window_ = nullptr;
    }
}

Window::Window(Window &&other) noexcept
    : callbacks_(other.callbacks_), sdl_window_(other.sdl_window_), gl_context_(other.gl_context_) {
    other.unregister_callbacks_();
    register_callbacks_();

    other.sdl_window_ = nullptr;
    other.gl_context_ = nullptr;
}

Window &Window::operator=(Window &&other) noexcept {
    if (this != &other) {
        unregister_callbacks_();
        other.unregister_callbacks_();
        std::swap(callbacks_, other.callbacks_);
        register_callbacks_();

        sdl_window_ = other.sdl_window_;
        other.sdl_window_ = nullptr;

        gl_context_ = other.gl_context_;
        other.gl_context_ = nullptr;
    }
    return *this;
}

glm::mat4 Window::projection() const {
    return glm::orthoZO(0.0f, static_cast<float>(size().x), static_cast<float>(size().y), 0.0f, 1.0f, 0.0f);
}

SDL_Window *Window::underlying() const {
    return sdl_window_;
}

SDL_GLContext Window::gl_context() const {
    return gl_context_;
}

void Window::make_context_current() {
    if (!SDL_GL_MakeCurrent(sdl_window_, gl_context_))
        MIZU_LOG_ERROR("Failed to make GL context current: {}", SDL_GetError());
}

void Window::swap() {
    if (!SDL_GL_SwapWindow(sdl_window_))
        MIZU_LOG_ERROR("Failed to swap window: {}", SDL_GetError());
}

glm::ivec2 Window::size() const {
    glm::ivec2 size;
    if (!SDL_GetWindowSize(sdl_window_, &size.x, &size.y))
        MIZU_LOG_ERROR("Failed to get size of window: {}", SDL_GetError());
    return size;
}

void Window::set_size(glm::ivec2 size) {
    if (!SDL_SetWindowSize(sdl_window_, size.x, size.y))
        MIZU_LOG_ERROR("Failed to set size of window: {}", SDL_GetError());
}

glm::ivec2 Window::pos() const {
    glm::ivec2 pos;
    if (!SDL_GetWindowPosition(sdl_window_, &pos.x, &pos.y))
        MIZU_LOG_ERROR("Failed to get pos of window: {}", SDL_GetError());
    return pos;
}

void Window::set_pos(glm::ivec2 pos) {
    if (!SDL_SetWindowPosition(sdl_window_, pos.x, pos.y))
        MIZU_LOG_ERROR("Failed to set pos of window: {}", SDL_GetError());
}

void Window::set_icon(SDL_Surface *icon) {
    if (!SDL_SetWindowIcon(sdl_window_, icon))
        MIZU_LOG_ERROR("Failed to set window icon: {}", SDL_GetError());
}

void Window::set_icon_dir(const std::filesystem::path &path) {
    SDL_Surface *icon = nullptr;
    for (const std::filesystem::directory_iterator dir(path); auto &entry: dir) {
        if (!entry.is_regular_file() || entry.path().extension() != ".png")
            continue;

        auto surf = read_image_to_sdl_surface(entry);
        if (!surf)
            continue;

        if (!icon)
            icon = surf;
        else if (!SDL_AddSurfaceAlternateImage(icon, surf))
            MIZU_LOG_ERROR("Failed to add alternate surface to window icon: {}", SDL_GetError());
    }

    if (!icon)
        MIZU_LOG_ERROR("No .png images in directory '{}' for window icon", path);
    else
        set_icon(icon);
}

Window::Window(SDL_Window *sdl_window, CallbackMgr &callbacks)
    : callbacks_(callbacks), sdl_window_(sdl_window) {
    register_callbacks_();

    gl_context_ = SDL_GL_CreateContext(sdl_window);
    if (!gl_context_) {
        MIZU_LOG_ERROR("Failed to create GL context: {}", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
    MIZU_LOG_DEBUG("Created GL context");
}

void Window::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPresent>(callback_id_, [&](const auto &) { swap(); });
}

void Window::unregister_callbacks_() {
    callbacks_.unsub<PPresent>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

WindowBuilder::WindowBuilder(const std::string &title)
    : WindowBuilder(title, glm::ivec2(0)) {}

WindowBuilder::WindowBuilder(const std::string &title, glm::ivec2 size)
    : title_(title),
      location_(static_cast<int>(SDL_WINDOWPOS_CENTERED), static_cast<int>(SDL_WINDOWPOS_CENTERED), size.x, size.y),
      display_idx_(0) {
    props_ = SDL_CreateProperties();
    if (props_ == 0)
        MIZU_LOG_ERROR("Failed to create SDL properties: {}", SDL_GetError());
    this->opengl();
}

WindowBuilder &WindowBuilder::fullscreen() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window fullscreen: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::opengl() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window opengl: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::hidden() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window hidden: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::borderless() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window borderless: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::resizable() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window resizable: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::minimized() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window minimized: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::maximized() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window maximized: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::mouse_grabbed() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window mouse grabbed: {}", SDL_GetError());
    return *this;
}

// window_builder &window_builder::external() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_EXTERNAL_GRAPHICS_CONTEXT_BOOLEAN, true);
//     return *this;
// }
//
// window_builder &window_builder::modal() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MODAL_BOOLEAN, true);
//     return *this;
// }

WindowBuilder &WindowBuilder::high_pixel_density() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window high pixel density: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::always_on_top() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window always on top: {}", SDL_GetError());
    return *this;
}

// window_builder &window_builder::utility() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_UTILITY_BOOLEAN, true);
//     return *this;
// }
//
// window_builder &window_builder::tooltip() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_TOOLTIP_BOOLEAN, true);
//     return *this;
// }
//
// window_builder &window_builder::popup_menu() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MENU_BOOLEAN, true);
//     return *this;
// }
//
// window_builder &window_builder::vulkan() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_VULKAN_BOOLEAN, true);
//     return *this;
// }
//
// window_builder &window_builder::metal() {
//     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_METAL_BOOLEAN, true);
//     return *this;
// }

WindowBuilder &WindowBuilder::transparent() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true))
        MIZU_LOG_ERROR("Failed to set SDL property, window transparent: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::not_focusable() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN, false))
        MIZU_LOG_ERROR("Failed to set SDL property, window not focusable: {}", SDL_GetError());
    return *this;
}

WindowBuilder &WindowBuilder::position(int x, int y) {
    location_.x = x;
    location_.y = y;
    return *this;
}

WindowBuilder &WindowBuilder::display(int idx) {
    display_idx_ = idx;
    return *this;
}

std::expected<Window, std::string> WindowBuilder::build(CallbackMgr &callbacks) {
    auto window_size_rect_result = get_window_size_rect();
    if (!window_size_rect_result)
        return std::unexpected(window_size_rect_result.error());
    auto window_size_rect = window_size_rect_result.value();

    if (!SDL_SetStringProperty(props_, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title_.c_str()))
        MIZU_LOG_ERROR("Failed to set SDL property, window create title: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_X_NUMBER, window_size_rect.x))
        MIZU_LOG_ERROR("Failed to set SDL property, window create x number: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_Y_NUMBER, window_size_rect.y))
        MIZU_LOG_ERROR("Failed to set SDL property, window create y number: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, window_size_rect.w))
        MIZU_LOG_ERROR("Failed to set SDL property, window create width: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, window_size_rect.h))
        MIZU_LOG_ERROR("Failed to set SDL property, window create height: {}", SDL_GetError());

    auto sdl_window = SDL_CreateWindowWithProperties(props_);
    if (!sdl_window) {
        MIZU_LOG_ERROR("Failed to create SDL window: {}", SDL_GetError());
        return std::unexpected(std::string(SDL_GetError()));
    }
    MIZU_LOG_DEBUG("Created SDL window");

    return Window(sdl_window, callbacks);
}

std::expected<SDL_DisplayID, std::string> WindowBuilder::get_display_id() {
    int display_count;
    auto displays = SDL_GetDisplays(&display_count);
    if (!displays)
        return std::unexpected(std::string(SDL_GetError()));

    if (display_count <= display_idx_) {
        MIZU_LOG_WARN("Requested display out of bounds ({} <= {}), using display 0", display_count, display_idx_);
        display_idx_ = 0;
    }

    SDL_DisplayID display_id = displays[display_idx_];
    SDL_free(displays);

    return display_id;
}

std::expected<SDL_Rect, std::string> WindowBuilder::get_window_size_rect() {
    auto display_id_result = get_display_id();
    if (!display_id_result)
        return std::unexpected(display_id_result.error());
    auto display_id = display_id_result.value();

    SDL_Rect display_bounds;
    if (!SDL_GetDisplayBounds(display_id, &display_bounds))
        return std::unexpected(std::string(SDL_GetError()));

    if (SDL_GetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, false))
        return display_bounds;

    if (location_.x == SDL_WINDOWPOS_CENTERED && location_.y == SDL_WINDOWPOS_CENTERED) {
        location_.x = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
        location_.y = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
    } else {
        location_.x += display_bounds.x;
        location_.y += display_bounds.y;
    }

    return location_;
}
} // namespace mizu
