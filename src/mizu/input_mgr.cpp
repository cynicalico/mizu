#include "mizu/input_mgr.hpp"
#include <SDL3/SDL_keyboard.h>
#include "mizu/log.hpp"

namespace mizu {
InputMgr::InputMgr(CallbackMgr &callbacks)
    : callbacks_(callbacks) {
    register_callbacks_();
}

InputMgr::~InputMgr() {
    unregister_callbacks_();
}

bool InputMgr::down(Key key, Mod mods) {
    if (const auto it = key_state_.find(key); it != key_state_.end())
        return it->second.pressed && is_flag_set(it->second.mods, mods);
    return false;
}

bool InputMgr::pressed(Key key, Mod mods) {
    if (const auto it = key_state_.find(key); it != key_state_.end()) {
        if (const auto it2 = prev_key_state_.find(key); it2 != prev_key_state_.end())
            return it->second.pressed && is_flag_set(it->second.mods, mods) && !it2->second.pressed;
        return it->second.pressed && is_flag_set(it->second.mods, mods);
    }
    return false;
}

bool InputMgr::released(Key key, Mod mods) {
    if (const auto it = key_state_.find(key); it != key_state_.end()) {
        if (const auto it2 = prev_key_state_.find(key); it2 != prev_key_state_.end())
            return !it->second.pressed && is_flag_set(it2->second.mods, mods) && it2->second.pressed;
        return false;
    }
    return false;
}

bool InputMgr::down(MouseButton button, Mod mods) {
    if (const auto it = mouse_button_state_.find(button); it != mouse_button_state_.end())
        return it->second.pressed && is_flag_set(it->second.mods, mods);
    return false;
}

bool InputMgr::pressed(MouseButton button, Mod mods) {
    if (const auto it = mouse_button_state_.find(button); it != mouse_button_state_.end()) {
        if (const auto it2 = prev_mouse_button_state_.find(button); it2 != prev_mouse_button_state_.end())
            return it->second.pressed && is_flag_set(it->second.mods, mods) && !it2->second.pressed;
        return it->second.pressed && is_flag_set(it->second.mods, mods);
    }
    return false;
}

bool InputMgr::released(MouseButton button, Mod mods) {
    if (const auto it = mouse_button_state_.find(button); it != mouse_button_state_.end()) {
        if (const auto it2 = prev_mouse_button_state_.find(button); it2 != prev_mouse_button_state_.end())
            return !it->second.pressed && is_flag_set(it2->second.mods, mods) && it2->second.pressed;
        return false;
    }
    return false;
}

float InputMgr::mouse_x() const {
    return mouse_pos_.x;
}

float InputMgr::mouse_y() const {
    return mouse_pos_.y;
}

float InputMgr::mouse_px() const {
    return prev_mouse_pos_.x;
}

float InputMgr::mouse_py() const {
    return prev_mouse_pos_.y;
}

float InputMgr::mouse_dx() const {
    return mouse_relative_.x;
}

float InputMgr::mouse_dy() const {
    return mouse_relative_.y;
}

float InputMgr::mouse_scroll_x() const {
    return mouse_scroll_.x;
}

float InputMgr::mouse_scroll_y() const {
    return mouse_scroll_.y;
}

void InputMgr::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPreUpdate>(callback_id_, [&](const auto &p) { update_(p.dt); });
    callbacks_.sub<PEventKeyDown>(callback_id_, [&](const auto &p) { key_down_(p.timestamp, p.key, p.mod); });
    callbacks_.sub<PEventKeyUp>(callback_id_, [&](const auto &p) { key_up_(p.timestamp, p.key, p.mod); });
    callbacks_.sub<PEventMouseMotion>(callback_id_, [&](const auto &p) {
        mouse_motion_(p.timestamp, p.x, p.y, p.dx, p.dy);
    });
    callbacks_.sub<PEventMouseButtonDown>(callback_id_, [&](const auto &p) {
        mouse_down_(p.timestamp, p.button, p.x, p.y);
    });
    callbacks_.sub<PEventMouseButtonUp>(callback_id_, [&](const auto &p) {
        mouse_up_(p.timestamp, p.button, p.x, p.y);
    });
    callbacks_.sub<PEventMouseWheel>(callback_id_, [&](const auto &p) {
        mouse_wheel_(p.timestamp, p.natural, p.x, p.y, p.mouse_x, p.mouse_y);
    });
}

void InputMgr::unregister_callbacks_() {
    callbacks_.unsub<PEventMouseWheel>(callback_id_);
    callbacks_.unsub<PEventMouseButtonUp>(callback_id_);
    callbacks_.unsub<PEventMouseButtonDown>(callback_id_);
    callbacks_.unsub<PEventMouseMotion>(callback_id_);
    callbacks_.unsub<PEventKeyUp>(callback_id_);
    callbacks_.unsub<PEventKeyDown>(callback_id_);
    callbacks_.unsub<PPreUpdate>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

void InputMgr::update_(double dt) {
    prev_key_state_.clear();
    for (auto &p: key_state_)
        prev_key_state_[p.first] = p.second;

    prev_mouse_button_state_.clear();
    for (auto &p: mouse_button_state_)
        prev_mouse_button_state_[p.first] = p.second;

    prev_mouse_pos_.x = mouse_pos_.x;
    prev_mouse_pos_.y = mouse_pos_.y;
    mouse_relative_.x = 0;
    mouse_relative_.y = 0;
    mouse_scroll_.x = 0;
    mouse_scroll_.y = 0;

    callbacks_.poll<PEventKeyDown>(callback_id_);
    callbacks_.poll<PEventKeyUp>(callback_id_);
    callbacks_.poll<PEventMouseMotion>(callback_id_);
    callbacks_.poll<PEventMouseButtonDown>(callback_id_);
    callbacks_.poll<PEventMouseButtonUp>(callback_id_);
    callbacks_.poll<PEventMouseWheel>(callback_id_);
}

void InputMgr::key_down_(std::uint64_t timestamp, SDL_Keycode sdl_key, SDL_Keymod sdl_mods) {
    const auto key = static_cast<Key>(sdl_key);
    key_state_[key].timestamp = timestamp;
    key_state_[key].pressed = true;
    key_state_[key].mods = static_cast<Mod>(sdl_mods);
}

void InputMgr::key_up_(std::uint64_t timestamp, SDL_Keycode sdl_key, SDL_Keymod sdl_mods) {
    const auto key = static_cast<Key>(sdl_key);
    key_state_[key].timestamp = timestamp;
    key_state_[key].pressed = false;
    key_state_[key].mods = static_cast<Mod>(sdl_mods);
}

void InputMgr::mouse_motion_(std::uint64_t, float x, float y, float dx, float dy) {
    mouse_pos_.x = x;
    mouse_pos_.y = y;
    mouse_relative_.x += dx;
    mouse_relative_.y += dy;
}

void InputMgr::mouse_down_(std::uint64_t timestamp, std::uint8_t sdl_button, float x, float y) {
    const auto button = static_cast<MouseButton>(sdl_button);
    mouse_button_state_[button].timestamp = timestamp;
    mouse_button_state_[button].pressed = true;
    mouse_button_state_[button].mods = static_cast<Mod>(SDL_GetModState());
}

void InputMgr::mouse_up_(std::uint64_t timestamp, std::uint8_t sdl_button, float x, float y) {
    const auto button = static_cast<MouseButton>(sdl_button);
    mouse_button_state_[button].timestamp = timestamp;
    mouse_button_state_[button].pressed = false;
    mouse_button_state_[button].mods = static_cast<Mod>(SDL_GetModState());
}

void InputMgr::mouse_wheel_(std::uint64_t timestamp, bool natural, float x, float y, float mouse_x, float mouse_y) {
    mouse_scroll_.x += x;
    mouse_scroll_.y += y;
}
} // namespace mizu
