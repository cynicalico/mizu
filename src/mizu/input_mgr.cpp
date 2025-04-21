#include "mizu/input_mgr.hpp"
#include "mizu/log.hpp"

namespace mizu {
InputMgr::InputMgr(CallbackMgr &callbacks) : callbacks_(callbacks) {
    register_callbacks_();
}

InputMgr::~InputMgr() {
    unregister_callbacks_();
}

bool InputMgr::down(Key key, Mod mods) {
    if (auto it = key_state_.find(key); it != key_state_.end())
        return it->second.pressed && is_flag_set(it->second.mods, mods);
    return false;
}

bool InputMgr::pressed(Key key, Mod mods) {
    auto it = key_state_.find(key);
    if (it != key_state_.end()) {
        auto it2 = prev_key_state_.find(key);
        if (it2 != prev_key_state_.end())
            return it->second.pressed && is_flag_set(it->second.mods, mods) && !it2->second.pressed;
        return true;
    }
    return false;
}

bool InputMgr::released(Key key, Mod mods) {
    auto it = key_state_.find(key);
    if (it != key_state_.end()) {
        auto it2 = prev_key_state_.find(key);
        if (it2 != prev_key_state_.end())
            return !it->second.pressed && is_flag_set(it2->second.mods, mods) && it2->second.pressed;
        return true;
    }
    return false;
}

void InputMgr::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPreUpdate>(callback_id_, [&](const auto &p) { update_(p.dt); });
    callbacks_.sub<PEventKeyDown>(callback_id_, [&](const auto &p) { key_down_(p.timestamp, p.key, p.mod); });
    callbacks_.sub<PEventKeyUp>(callback_id_, [&](const auto &p) { key_up_(p.timestamp, p.key, p.mod); });
}

void InputMgr::unregister_callbacks_() {
    callbacks_.unsub<PPreUpdate>(callback_id_);
    callbacks_.unsub<PEventKeyDown>(callback_id_);
    callbacks_.unsub<PEventKeyUp>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

void InputMgr::update_(double dt) {
    prev_key_state_.clear();
    for (auto &p: key_state_) prev_key_state_[p.first] = p.second;

    callbacks_.poll<PEventKeyDown>(callback_id_);
    callbacks_.poll<PEventKeyUp>(callback_id_);
}

void InputMgr::key_down_(std::uint64_t timestamp, SDL_Keycode sdl_key, SDL_Keymod sdl_mods) {
    auto key = static_cast<Key>(sdl_key);
    key_state_[key].timestamp = timestamp;
    key_state_[key].pressed = true;
    key_state_[key].mods = static_cast<Mod>(sdl_mods);
}

void InputMgr::key_up_(std::uint64_t timestamp, SDL_Keycode sdl_key, SDL_Keymod sdl_mods) {
    auto key = static_cast<Key>(sdl_key);
    key_state_[key].timestamp = timestamp;
    key_state_[key].pressed = false;
    key_state_[key].mods = static_cast<Mod>(sdl_mods);
}
} // namespace mizu
