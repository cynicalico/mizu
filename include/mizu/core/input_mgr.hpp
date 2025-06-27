#ifndef MIZU_INPUT_MGR_HPP
#define MIZU_INPUT_MGR_HPP

#include <glm/vec2.hpp>
#include "mizu/core/callback_mgr.hpp"
#include "mizu/core/input_types.hpp"
#include "mizu/core/window.hpp"
#include "payloads.hpp"

namespace mizu {
class InputMgr {
public:
    InputMgr(CallbackMgr &callbacks, Window *window);

    ~InputMgr();

    InputMgr(const InputMgr &) = delete;
    InputMgr &operator=(const InputMgr &) = delete;

    InputMgr(InputMgr &&other) noexcept = delete;
    InputMgr &operator=(InputMgr &&other) = delete;

    void start_text_input();
    void stop_text_input();

    bool down(Key key, Mod mods = Mod::None);
    bool pressed(Key key, Mod mods = Mod::None);
    bool released(Key key, Mod mods = Mod::None);

    bool down(MouseButton button, Mod mods = Mod::None);
    bool pressed(MouseButton button, Mod mods = Mod::None);
    bool released(MouseButton button, Mod mods = Mod::None);

    float mouse_x() const;
    float mouse_y() const;
    float mouse_px() const;
    float mouse_py() const;
    float mouse_dx() const;
    float mouse_dy() const;
    float mouse_scroll_x() const;
    float mouse_scroll_y() const;

private:
    Window *window_;

    struct KeyState {
        std::uint64_t timestamp;
        bool pressed;
        Mod mods;
    };

    std::unordered_map<Key, KeyState> key_state_{};
    std::unordered_map<Key, KeyState> prev_key_state_{};

    struct MouseButtonState {
        std::uint64_t timestamp;
        bool pressed;
        Mod mods;
    };

    std::unordered_map<MouseButton, MouseButtonState> mouse_button_state_{};
    std::unordered_map<MouseButton, MouseButtonState> prev_mouse_button_state_{};

    glm::vec2 mouse_pos_{};
    glm::vec2 prev_mouse_pos_{};
    glm::vec2 mouse_relative_{};
    glm::vec2 mouse_scroll_{};

    std::size_t callback_id_{0};
    CallbackMgr &callbacks_;

    void register_callbacks_();
    void unregister_callbacks_();

    void update_(double dt);

    void key_down_(std::uint64_t timestamp, SDL_Keycode sdl_key, SDL_Keymod sdl_mods);
    void key_up_(std::uint64_t timestamp, SDL_Keycode sdl_key, SDL_Keymod sdl_mods);
    void mouse_motion_(std::uint64_t timestamp, float x, float y, float dx, float dy);
    void mouse_down_(std::uint64_t timestamp, std::uint8_t sdl_button, float x, float y);
    void mouse_up_(std::uint64_t timestamp, std::uint8_t sdl_button, float x, float y);
    void mouse_wheel_(std::uint64_t timestamp, bool natural, float x, float y, float mouse_x, float mouse_y);
};
} // namespace mizu

#endif // MIZU_INPUT_MGR_HPP
