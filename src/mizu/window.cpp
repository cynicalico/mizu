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
        title_(title), width_(width), height_(height), pos_x_(SDL_WINDOWPOS_UNDEFINED),
        pos_y_(SDL_WINDOWPOS_UNDEFINED), display_idx_(0) {
        props_ = SDL_CreateProperties();
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
    }

    window_builder &window_builder::fullscreen() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
        return *this;
    }

    // window_builder &window_builder::opengl() {
    //     SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);
    //     return *this;
    // }

    window_builder &window_builder::hidden() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
        return *this;
    }

    window_builder &window_builder::borderless() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
        return *this;
    }

    window_builder &window_builder::resizable() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
        return *this;
    }

    window_builder &window_builder::minimized() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN, true);
        return *this;
    }

    window_builder &window_builder::maximized() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, true);
        return *this;
    }

    window_builder & window_builder::mouse_grabbed() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN, true);
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
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true);
        return *this;
    }

    window_builder &window_builder::always_on_top() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);
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
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN, true);
        return *this;
    }

    window_builder &window_builder::not_focusable() {
        SDL_SetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN, false);
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

        auto window_properties = SDL_CreateProperties();
        if (window_properties == 0) {
            return std::unexpected(std::string(SDL_GetError()));
        }

        auto window_size_rect_result = get_window_size_rect();
        if (!window_size_rect_result.has_value()) {
            return std::unexpected(window_size_rect_result.error());
        }
        auto window_size_rect = window_size_rect_result.value();

        SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_X_NUMBER, window_size_rect.x);
        SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_Y_NUMBER, window_size_rect.y);
        SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, window_size_rect.w);
        SDL_SetNumberProperty(props_, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, window_size_rect.h);

        auto sdl_window = SDL_CreateWindowWithProperties(props_);
        if (!sdl_window) {
            SPDLOG_ERROR("Failed to create SDL window: {}", SDL_GetError());
            return std::unexpected(std::string(SDL_GetError()));
        }

        return window(sdl_window);
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

        SDL_DisplayID display_id = displays[display_idx_];
        SDL_free(displays);

        return display_id;
    }

    std::expected<SDL_Rect, std::string> window_builder::get_window_size_rect() {
        auto display_id_result = get_display_id();
        if (!display_id_result.has_value()) {
            return std::unexpected(display_id_result.error());
        }
        auto display_id = display_id_result.value();

        SDL_Rect display_bounds;
        if (!SDL_GetDisplayBounds(display_id, &display_bounds)) {
            return std::unexpected(std::string(SDL_GetError()));
        }

        if (SDL_GetBooleanProperty(props_, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, false)) {
            return display_bounds;
        }

        SDL_Rect window_size_rect;
        if (pos_x_ == SDL_WINDOWPOS_CENTERED && pos_y_ == SDL_WINDOWPOS_CENTERED) {
            window_size_rect.x = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
            window_size_rect.y = SDL_WINDOWPOS_CENTERED_DISPLAY(display_id);
        } else {
            window_size_rect.x = display_bounds.x + pos_x_;
            window_size_rect.y = display_bounds.y + pos_y_;
        }
        window_size_rect.w = width_;
        window_size_rect.h = height_;

        return window_size_rect;
    }
} // namespace mizu
