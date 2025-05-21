#ifndef MIZU_WINDOW_HPP
#define MIZU_WINDOW_HPP

#include <SDL3/SDL_video.h>
#include <expected>
#include <filesystem>
#include <string>
#include "mizu/callback_mgr.hpp"
#include "mizu/types.hpp"

namespace mizu {
class Window {
    friend class WindowBuilder;

public:
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(Window &&other) noexcept;
    Window &operator=(Window &&other) noexcept;

    SDL_GLContext gl_context() const;
    void make_context_current();

    void swap();

    Size2d<int> get_size() const;
    void set_size(Size2d<int> size);

    Pos2d<int> get_pos() const;
    void set_pos(Pos2d<int> pos);

    void set_icon(SDL_Surface *icon);
    void set_icon_dir(const std::filesystem::path &path);

private:
    CallbackMgr &callbacks_;
    std::size_t callback_id_;

    SDL_Window *sdl_window_;
    SDL_GLContext gl_context_;

    explicit Window(SDL_Window *sdl_window, CallbackMgr &callbacks);

    void register_callbacks_();
    void unregister_callbacks_();
};

class WindowBuilder {
public:
    WindowBuilder(const std::string &title, Size2d<int> size);
    explicit WindowBuilder(const std::string &title);

    ~WindowBuilder() = default;

    WindowBuilder(const WindowBuilder &) = delete;
    WindowBuilder &operator=(const WindowBuilder &) = delete;

    WindowBuilder(WindowBuilder &&other) noexcept = delete;
    WindowBuilder &operator=(WindowBuilder &&other) = delete;

    WindowBuilder &fullscreen();
    WindowBuilder &opengl();
    WindowBuilder &hidden();
    WindowBuilder &borderless();
    WindowBuilder &resizable();
    WindowBuilder &minimized();
    WindowBuilder &maximized();
    WindowBuilder &mouse_grabbed();

    // window_builder &external();
    // window_builder &modal();

    WindowBuilder &high_pixel_density();
    WindowBuilder &always_on_top();

    // window_builder &utility();
    // window_builder &tooltip();
    // window_builder &popup_menu();
    // window_builder &vulkan();
    // window_builder &metal();

    WindowBuilder &transparent();
    WindowBuilder &not_focusable();

    WindowBuilder &position(int x, int y);
    WindowBuilder &display(int idx);

    [[nodiscard]] std::expected<Window, std::string> build(CallbackMgr &callbacks);

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
