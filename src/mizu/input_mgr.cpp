#include "mizu/input_mgr.hpp"
#include "mizu/log.hpp"

namespace mizu {
InputMgr::InputMgr(CallbackMgr &callbacks) : callbacks_(callbacks) { register_callbacks_(); }

InputMgr::~InputMgr() { unregister_callbacks_(); }

void InputMgr::register_callbacks_() {
    callback_id_ = callbacks_.reg();
    callbacks_.sub<PPreUpdate>(callback_id_, [&](const auto &p) { update_(p.dt); });
    callbacks_.sub<PEventKeyDown>(callback_id_, [&](const auto &p) { key_down_(p); });
    callbacks_.sub<PEventKeyUp>(callback_id_, [&](const auto &p) { key_up_(p); });
}

void InputMgr::unregister_callbacks_() {
    callbacks_.unsub<PPreUpdate>(callback_id_);
    callbacks_.unsub<PEventKeyDown>(callback_id_);
    callbacks_.unsub<PEventKeyUp>(callback_id_);
    callbacks_.unreg(callback_id_);
    callback_id_ = 0;
}

void InputMgr::update_(double dt) {
    callbacks_.poll<PEventKeyDown>(callback_id_);
    callbacks_.poll<PEventKeyUp>(callback_id_);
}

void InputMgr::key_down_(const PEventKeyDown &p) { SPDLOG_DEBUG("InputMgr::key_down_: {}", (int)p.scancode); }

void InputMgr::key_up_(const PEventKeyUp &p) { SPDLOG_DEBUG("InputMgr::key_up_: {}", (int)p.scancode); }
} // namespace mizu
