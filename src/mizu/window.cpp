#include "mizu/window.hpp"
#include <SDL3/SDL.h>
#include "mizu/log.hpp"

namespace mizu {
Window::~Window() {
    if (gl_context_) {
        if (!SDL_GL_DestroyContext(gl_context_))
            SPDLOG_ERROR("Failed to destroy GL context: {}", SDL_GetError());
        else
            SPDLOG_DEBUG("Destroyed GL context");
        gl_context_ = nullptr;
    }

    if (sdl_window_) {
        SDL_DestroyWindow(sdl_window_);
        SPDLOG_DEBUG("Destroyed SDL window");
        sdl_window_ = nullptr;
    }
}

Window::Window(Window &&other) noexcept : sdl_window_(other.sdl_window_), gl_context_(other.gl_context_) {
    other.sdl_window_ = nullptr;
    other.gl_context_ = nullptr;
}

Window &Window::operator=(Window &&other) noexcept {
    if (this != &other) {
        sdl_window_ = other.sdl_window_;
        gl_context_ = other.gl_context_;

        other.sdl_window_ = nullptr;
        other.gl_context_ = nullptr;
    }
    return *this;
}

SDL_GLContext Window::gl_context() const { return gl_context_; }

void Window::make_context_current() {
    if (!SDL_GL_MakeCurrent(sdl_window_, gl_context_))
        SPDLOG_ERROR("Failed to make GL context current: {}", SDL_GetError());
}

void Window::swap() {
    if (!SDL_GL_SwapWindow(sdl_window_))
        SPDLOG_ERROR("Failed to swap window: {}", SDL_GetError());
}

Size2d<int> Window::get_size() const {
    Size2d<int> size;
    if (!SDL_GetWindowSize(sdl_window_, &size.w, &size.h))
        SPDLOG_ERROR("Failed to get size of window: {}", SDL_GetError());
    return size;
}

void Window::set_size(Size2d<int> size) {
    if (!SDL_SetWindowSize(sdl_window_, size.w, size.h))
        SPDLOG_ERROR("Failed to set size of window: {}", SDL_GetError());
}

Pos2d<int> Window::get_pos() const {
    Pos2d<int> pos;
    if (!SDL_GetWindowPosition(sdl_window_, &pos.x, &pos.y))
        SPDLOG_ERROR("Failed to get pos of window: {}", SDL_GetError());
    return pos;
}

void Window::set_pos(Pos2d<int> pos) {
    if (!SDL_SetWindowPosition(sdl_window_, pos.x, pos.y))
        SPDLOG_ERROR("Failed to set pos of window: {}", SDL_GetError());
}

Window::Window(SDL_Window *sdl_window) : sdl_window_(sdl_window) {
    gl_context_ = SDL_GL_CreateContext(sdl_window);
    if (!gl_context_) {
        SPDLOG_ERROR("Failed to create GL context: {}", SDL_GetError());
        std::exit(EXIT_FAILURE);
    }
    SPDLOG_DEBUG("Created GL context");
}

window_builder::window_builder(const std::string &title) : window_builder(title, Size2d<int>()) {}

window_builder::window_builder(const std::string &title, Size2d<int> size)
    : title_(title),
      location_(static_cast<int>(SDL_WINDOWPOS_CENTERED), static_cast<int>(SDL_WINDOWPOS_CENTERED), size.w, size.h),
      display_idx_(0) {
    props_ = SDL_CreateProperties();
    if (props_ == 0)
        SPDLOG_ERROR("Failed to create SDL properties: {}", SDL_GetError());
    this->opengl();
}

window_builder &window_builder::fullscreen() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window fullscreen: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::opengl() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window opengl: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::hidden() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window hidden: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::borderless() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window borderless: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::resizable() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window resizable: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::minimized() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window minimized: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::maximized() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window maximized: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::mouse_grabbed() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window mouse grabbed: {}", SDL_GetError());
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

window_builder &window_builder::high_pixel_density() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window high pixel density: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::always_on_top() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window always on top: {}", SDL_GetError());
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

window_builder &window_builder::transparent() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true))
        SPDLOG_ERROR("Failed to set SDL property, window transparent: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::not_focusable() {
    if (!SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN, false))
        SPDLOG_ERROR("Failed to set SDL property, window not focusable: {}", SDL_GetError());
    return *this;
}

window_builder &window_builder::position(int x, int y) {
    location_.x = x;
    location_.y = y;
    return *this;
}

window_builder &window_builder::display(int idx) {
    display_idx_ = idx;
    return *this;
}

std::expected<Window, std::string> window_builder::build() {
    auto window_size_rect_result = get_window_size_rect();
    if (!window_size_rect_result)
        return std::unexpected(window_size_rect_result.error());
    auto window_size_rect = window_size_rect_result.value();

    if (!SDL_SetStringProperty(props_, SDL_PROP_WINDOW_CREATE_TITLE_STRING, title_.c_str()))
        SPDLOG_ERROR("Failed to set SDL property, window create title: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_X_NUMBER, window_size_rect.x))
        SPDLOG_ERROR("Failed to set SDL property, window create x number: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_Y_NUMBER, window_size_rect.y))
        SPDLOG_ERROR("Failed to set SDL property, window create y number: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, window_size_rect.w))
        SPDLOG_ERROR("Failed to set SDL property, window create width: {}", SDL_GetError());

    if (!SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, window_size_rect.h))
        SPDLOG_ERROR("Failed to set SDL property, window create height: {}", SDL_GetError());

    auto sdl_window = SDL_CreateWindowWithProperties(props_);
    if (!sdl_window) {
        SPDLOG_ERROR("Failed to create SDL window: {}", SDL_GetError());
        return std::unexpected(std::string(SDL_GetError()));
    }
    SPDLOG_DEBUG("Created SDL window");

    return Window(sdl_window);
}

std::expected<SDL_DisplayID, std::string> window_builder::get_display_id() {
    int display_count;
    auto displays = SDL_GetDisplays(&display_count);
    if (!displays)
        return std::unexpected(std::string(SDL_GetError()));

    if (display_count <= display_idx_) {
        SPDLOG_WARN("Requested display out of bounds ({} <= {}), using display 0", display_count, display_idx_);
        display_idx_ = 0;
    }

    SDL_DisplayID display_id = displays[display_idx_];
    SDL_free(displays);

    return display_id;
}

std::expected<SDL_Rect, std::string> window_builder::get_window_size_rect() {
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
