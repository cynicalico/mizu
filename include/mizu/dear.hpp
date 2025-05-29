#ifndef MIZU_DEAR_HPP
#define MIZU_DEAR_HPP

#include <SDL3/SDL_events.h>
#include <imgui.h>
#include "mizu/callback_mgr.hpp"
#include "mizu/class_helpers.hpp"
#include "mizu/window.hpp"

namespace mizu {
class Dear {
public:
    Dear(CallbackMgr &callbacks, Window *window);
    ~Dear();

    NO_COPY(Dear)
    NO_MOVE(Dear)

    void process_sdl3_event(const SDL_Event &event);
    bool ignore_mouse_inputs() const;
    bool ignore_keyboard_inputs() const;

private:
    ImGuiContext *ctx_;
    ImGuiIO *io_;

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void pre_draw_();
    void draw_();

    void register_callbacks_();
    void unregister_callbacks_();
};
} // namespace mizu

#endif // MIZU_DEAR_HPP
