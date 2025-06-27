#include "mizu/core/application.hpp"
#include "mizu/core/engine.hpp"
#include "mizu/core/payloads.hpp"

namespace mizu {
Application::Application(Engine *engine)
    : engine(engine) {
    register_callbacks_();
}

Application::~Application() {
    unregister_callbacks_();
    engine = nullptr;
}

void Application::key_press_callback(Key key, Mod mods) {}

void Application::key_release_callback(Key key, Mod mods) {}

void Application::mouse_move_callback(float x, float y, float dx, float dy) {}

void Application::mouse_scroll_callback(float x, float y) {}

void Application::mouse_press_callback(MouseButton button, Mod mods) {}

void Application::mouse_release_callback(MouseButton button, Mod mods) {}

void Application::mouse_enter_callback() {}

void Application::mouse_exit_callback() {}

void Application::text_input_callback(const char *text) {}

void Application::pre_update_() {
    engine->callbacks.poll<PEventKeyDown>(callback_id);
    engine->callbacks.poll<PEventKeyUp>(callback_id);
    engine->callbacks.poll<PEventMouseMotion>(callback_id);
    engine->callbacks.poll<PEventMouseWheel>(callback_id);
    engine->callbacks.poll<PEventMouseButtonDown>(callback_id);
    engine->callbacks.poll<PEventMouseButtonUp>(callback_id);
    engine->callbacks.poll<PEventMouseEnter>(callback_id);
    engine->callbacks.poll<PEventMouseLeave>(callback_id);
    engine->callbacks.poll<PEventTextInput>(callback_id);
}

void Application::register_callbacks_() {
    callback_id = engine->callbacks.reg();
    engine->callbacks.sub<PPreUpdate>(callback_id, [&](const auto &p) { pre_update_(); });
    engine->callbacks.sub<PUpdate>(callback_id, [&](const auto &p) { update(p.dt); });
    engine->callbacks.sub<PDraw>(callback_id, [&](const auto &) { draw(); });
    engine->callbacks.sub<PEventKeyDown>(
            callback_id, [&](const auto &p) { key_press_callback(static_cast<Key>(p.key), static_cast<Mod>(p.mod)); });
    engine->callbacks.sub<PEventKeyUp>(callback_id, [&](const auto &p) {
        key_release_callback(static_cast<Key>(p.key), static_cast<Mod>(p.mod));
    });
    engine->callbacks.sub<PEventMouseMotion>(
            callback_id, [&](const auto &p) { mouse_move_callback(p.x, p.y, p.dx, p.dy); });
    engine->callbacks.sub<PEventMouseWheel>(callback_id, [&](const auto &p) { mouse_scroll_callback(p.x, p.y); });
    engine->callbacks.sub<PEventMouseButtonDown>(callback_id, [&](const auto &p) {
        mouse_press_callback(static_cast<MouseButton>(p.button), static_cast<Mod>(SDL_GetModState()));
    });
    engine->callbacks.sub<PEventMouseButtonUp>(callback_id, [&](const auto &p) {
        mouse_release_callback(static_cast<MouseButton>(p.button), static_cast<Mod>(SDL_GetModState()));
    });
    engine->callbacks.sub<PEventMouseEnter>(callback_id, [&](const auto &p) { mouse_enter_callback(); });
    engine->callbacks.sub<PEventMouseLeave>(callback_id, [&](const auto &p) { mouse_exit_callback(); });
    engine->callbacks.sub<PEventTextInput>(callback_id, [&](const auto &p) { text_input_callback(p.text); });
}

void Application::unregister_callbacks_() {
    engine->callbacks.unsub<PEventTextInput>(callback_id);
    engine->callbacks.unsub<PEventMouseLeave>(callback_id);
    engine->callbacks.unsub<PEventMouseEnter>(callback_id);
    engine->callbacks.unsub<PEventMouseButtonUp>(callback_id);
    engine->callbacks.unsub<PEventMouseButtonDown>(callback_id);
    engine->callbacks.unsub<PEventMouseWheel>(callback_id);
    engine->callbacks.unsub<PEventMouseMotion>(callback_id);
    engine->callbacks.unsub<PEventKeyUp>(callback_id);
    engine->callbacks.unsub<PEventKeyDown>(callback_id);
    engine->callbacks.unsub<PDraw>(callback_id);
    engine->callbacks.unsub<PUpdate>(callback_id);
    engine->callbacks.unsub<PPreUpdate>(callback_id);
    engine->callbacks.unreg(callback_id);
    callback_id = 0;
}
} // namespace mizu
