#include "mizu/window.hpp"
#include <SDL3/SDL.h>
#include "mizu/log.hpp"

std::atomic_uint open_window_count;
std::once_flag initialize_sdl;

namespace mizu {
    window::~window() {
        if (sdl_window_) {
            SDL_DestroyWindow(sdl_window_);
            sdl_window_ = nullptr;

            SDL_GL_DestroyContext(gl_context_);
            gl_context_ = nullptr;

            if (--open_window_count == 0) {
                SDL_Quit();
                SPDLOG_DEBUG("Quit SDL");
            }
        }
    }

    window::window(window &&other) noexcept : sdl_window_(other.sdl_window_), gl_context_(other.gl_context_) {
        other.sdl_window_ = nullptr;
        other.gl_context_ = nullptr;
    }

    window &window::operator=(window &&other) noexcept {
        if (this != &other) {
            sdl_window_ = other.sdl_window_;
            gl_context_ = other.gl_context_;

            other.sdl_window_ = nullptr;
            other.gl_context_ = nullptr;
        }
        return *this;
    }

    SDL_GLContext window::get_context() const { return gl_context_; }

    void window::swap() {
        if (!SDL_GL_SwapWindow(sdl_window_)) {
            SPDLOG_ERROR("Failed to swap window: {}", SDL_GetError());
        }
    }

    window::window(SDL_Window *sdl_window) : sdl_window_(sdl_window) {
        ++open_window_count;
        gl_context_ = SDL_GL_CreateContext(sdl_window);
    }

    window_builder::window_builder(const std::string &title) : window_builder(title, 0, 0) {}

    window_builder::window_builder(const std::string &title, int width, int height) :
        title_(title), width_(width), height_(height), window_flags_(0), pos_x_(SDL_WINDOWPOS_UNDEFINED),
        pos_y_(SDL_WINDOWPOS_UNDEFINED), display_idx_(0) {}

    window_builder &window_builder::fullscreen() {
        window_flags_ |= SDL_WINDOW_FULLSCREEN;
        return *this;
    }

    window_builder &window_builder::opengl() {
        window_flags_ |= SDL_WINDOW_OPENGL;
        return *this;
    }

    window_builder &window_builder::occluded() {
        window_flags_ |= SDL_WINDOW_OCCLUDED;
        return *this;
    }

    window_builder &window_builder::hidden() {
        window_flags_ |= SDL_WINDOW_HIDDEN;
        return *this;
    }

    window_builder &window_builder::borderless() {
        window_flags_ |= SDL_WINDOW_BORDERLESS;
        return *this;
    }

    window_builder &window_builder::resizable() {
        window_flags_ |= SDL_WINDOW_RESIZABLE;
        return *this;
    }

    window_builder &window_builder::minimized() {
        window_flags_ |= SDL_WINDOW_MINIMIZED;
        return *this;
    }

    window_builder &window_builder::maximized() {
        window_flags_ |= SDL_WINDOW_MAXIMIZED;
        return *this;
    }

    window_builder &window_builder::mouse_grabbed() {
        window_flags_ |= SDL_WINDOW_MOUSE_GRABBED;
        return *this;
    }

    window_builder &window_builder::input_focus() {
        window_flags_ |= SDL_WINDOW_INPUT_FOCUS;
        return *this;
    }

    window_builder &window_builder::mouse_focus() {
        window_flags_ |= SDL_WINDOW_MOUSE_FOCUS;
        return *this;
    }

    window_builder &window_builder::external() {
        window_flags_ |= SDL_WINDOW_EXTERNAL;
        return *this;
    }

    window_builder &window_builder::modal() {
        window_flags_ |= SDL_WINDOW_MODAL;
        return *this;
    }

    window_builder &window_builder::high_pixel_density() {
        window_flags_ |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
        return *this;
    }

    window_builder &window_builder::mouse_capture() {
        window_flags_ |= SDL_WINDOW_MOUSE_CAPTURE;
        return *this;
    }

    window_builder &window_builder::mouse_relative_mode() {
        window_flags_ |= SDL_WINDOW_MOUSE_RELATIVE_MODE;
        return *this;
    }

    window_builder &window_builder::always_on_top() {
        window_flags_ |= SDL_WINDOW_ALWAYS_ON_TOP;
        return *this;
    }

    window_builder &window_builder::utility() {
        window_flags_ |= SDL_WINDOW_UTILITY;
        return *this;
    }

    window_builder &window_builder::tooltip() {
        window_flags_ |= SDL_WINDOW_TOOLTIP;
        return *this;
    }

    window_builder &window_builder::popup_menu() {
        window_flags_ |= SDL_WINDOW_POPUP_MENU;
        return *this;
    }

    window_builder &window_builder::keyboard_grabbed() {
        window_flags_ |= SDL_WINDOW_KEYBOARD_GRABBED;
        return *this;
    }

    window_builder &window_builder::vulkan() {
        window_flags_ |= SDL_WINDOW_VULKAN;
        return *this;
    }

    window_builder &window_builder::metal() {
        window_flags_ |= SDL_WINDOW_METAL;
        return *this;
    }

    window_builder &window_builder::transparent() {
        window_flags_ |= SDL_WINDOW_TRANSPARENT;
        return *this;
    }

    window_builder &window_builder::not_focusable() {
        window_flags_ |= SDL_WINDOW_NOT_FOCUSABLE;
        return *this;
    }

    window_builder &window_builder::position(int x, int y) {
        pos_x_ = x;
        pos_y_ = y;
        return *this;
    }

    window_builder &window_builder::position_centered() {
        pos_x_ = SDL_WINDOWPOS_CENTERED;
        pos_y_ = SDL_WINDOWPOS_CENTERED;
        return *this;
    }

    window_builder &window_builder::display(int idx) {
        display_idx_ = idx;
        return *this;
    }

    std::expected<window, std::string> window_builder::build() {
        std::call_once(initialize_sdl, [&] {
            if (!SDL_Init(SDL_INIT_VIDEO)) {
                SPDLOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
                std::exit(EXIT_FAILURE);
            }

            int sdl_version = SDL_GetVersion();
            SPDLOG_DEBUG("Initialized SDL v{}.{}.{}", SDL_VERSIONNUM_MAJOR(sdl_version),
                         SDL_VERSIONNUM_MINOR(sdl_version), SDL_VERSIONNUM_MICRO(sdl_version));
        });

        if ((window_flags_ & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN) {
            return open_fullscreen_();
        }
        return open_windowed_();
    }

    std::expected<SDL_DisplayID, std::string> window_builder::get_display_id() {
        int display_count;
        auto displays = SDL_GetDisplays(&display_count);
        if (!displays) {
            return std::unexpected(std::string(SDL_GetError()));
        }

        if (display_count <= display_idx_) {
            SPDLOG_WARN("Requested display out of bounds ({} <= {}), using display 0", display_count, display_idx_);
            display_idx_ = 0;
        }

        return displays[display_idx_];
    }

    std::expected<window, std::string> window_builder::open_fullscreen_() {
        auto sdl_window = SDL_CreateWindow(title_.c_str(), 1, 1, window_flags_ ^ SDL_WINDOW_FULLSCREEN);
        if (!sdl_window) {
            return std::unexpected(std::string(SDL_GetError()));
        }

        auto display_id_result = get_display_id();
        if (!display_id_result.has_value()) {
            return std::unexpected(display_id_result.error());
        }
        auto display_id = display_id_result.value();

        SDL_Rect display_bounds;
        if (SDL_GetDisplayBounds(display_id, &display_bounds)) {
            if (!SDL_SetWindowPosition(sdl_window, display_bounds.x, display_bounds.y)) {
                SPDLOG_WARN("Failed to set window position: {}", SDL_GetError());
            }
            if (!SDL_SetWindowSize(sdl_window, display_bounds.w, display_bounds.h)) {
                SPDLOG_WARN("Failed to set window size: {}", SDL_GetError());
            }
            if (!SDL_SetWindowFullscreen(sdl_window, true)) {
                SPDLOG_WARN("Failed to set window fullscreen: {}", SDL_GetError());
            }
        } else {
            SPDLOG_WARN("Failed to get display bounds: {}", SDL_GetError());
        }

        return window(sdl_window);
    }

    std::expected<window, std::string> window_builder::open_windowed_() {
        auto sdl_window = SDL_CreateWindow(title_.c_str(), width_, height_, window_flags_);
        if (!sdl_window) {
            return std::unexpected(std::string(SDL_GetError()));
        }

        auto display_id_result = get_display_id();
        if (!display_id_result.has_value()) {
            return std::unexpected(display_id_result.error());
        }
        auto display_id = display_id_result.value();

        SDL_Rect display_bounds;
        if (SDL_GetDisplayBounds(display_id, &display_bounds)) {
            int window_x, window_y;
            if (pos_x_ == SDL_WINDOWPOS_CENTERED && pos_y_ == SDL_WINDOWPOS_CENTERED) {
                window_x = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
                window_y = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
            } else {
                window_x = display_bounds.x + pos_x_;
                window_y = display_bounds.y + pos_y_;
            }

            if (!SDL_SetWindowPosition(sdl_window, window_x, window_y)) {
                SPDLOG_WARN("Failed to set window position: {}", SDL_GetError());
            }
        } else {
            SPDLOG_WARN("Failed to get display bounds: {}", SDL_GetError());
        }

        return window(sdl_window);
    }
} // namespace mizu
