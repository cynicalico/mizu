#include "mizu/application.hpp"
#include "mizu/engine.hpp"
#include "mizu/payloads.hpp"

namespace mizu {
Application::Application(Engine *engine) : engine(engine) {
    register_callbacks_();
}

Application::~Application() {
    unregister_callbacks_();
    engine = nullptr;
}

void Application::register_callbacks_() {
    callback_id = engine->callbacks.reg();
    engine->callbacks.sub<PUpdate>(callback_id, [&](const auto &p) { update(p.dt); });
    engine->callbacks.sub<PDraw>(callback_id, [&](const auto &) { draw(); });
}

void Application::unregister_callbacks_() {
    engine->callbacks.unsub<PUpdate>(callback_id);
    engine->callbacks.unsub<PDraw>(callback_id);
    engine->callbacks.unreg(callback_id);
    callback_id = 0;
}
} // namespace mizu
