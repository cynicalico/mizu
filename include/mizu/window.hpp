#ifndef MIZU_WINDOW_HPP
#define MIZU_WINDOW_HPP

#include <SDL3/SDL_video.h>
#include <expected>
#include <string>
#include "types.hpp"

namespace mizu {
class Window {
    friend class window_builder;

public:
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&other) noexcept;
    Window &operator=(Window &&other) noexcept;

    SDL_GLContext get_context() const;
    void make_context_current();

    void swap();

    Size2d<int> get_size() const;
    void set_size(Size2d<int> size);

    Pos2d<int> get_pos() const;
    void set_pos(Pos2d<int> pos);

private:
    SDL_Window *sdl_window_;
    SDL_GLContext gl_context_;

    explicit Window(SDL_Window *sdl_window);
};

class window_builder {
public:
    window_builder(const std::string &title, Size2d<int> size);
    explicit window_builder(const std::string &title);

    ~window_builder() = default;

    window_builder(const window_builder &) = delete;
    window_builder &operator=(const window_builder &) = delete;

    window_builder(window_builder &&other) noexcept = delete;
    window_builder &operator=(window_builder &&other) = delete;

    window_builder &fullscreen();
    window_builder &opengl();
    window_builder &hidden();
    window_builder &borderless();
    window_builder &resizable();
    window_builder &minimized();
    window_builder &maximized();
    window_builder &mouse_grabbed();

    // window_builder &external();
    // window_builder &modal();

    window_builder &high_pixel_density();
    window_builder &always_on_top();

    // window_builder &utility();
    // window_builder &tooltip();
    // window_builder &popup_menu();
    // window_builder &vulkan();
    // window_builder &metal();

    window_builder &transparent();
    window_builder &not_focusable();

    window_builder &position(int x, int y);
    window_builder &display(int idx);

    [[nodiscard]] std::expected<Window, std::string> build();

private:
    std::string title_;
    SDL_Rect location_;
    int display_idx_;
    SDL_PropertiesID props_;

    std::expected<SDL_DisplayID, std::string> get_display_id();

    std::expected<SDL_Rect, std::string> get_window_size_rect();
};
} // namespace mizu

#endif // MIZU_WINDOW_HPP
