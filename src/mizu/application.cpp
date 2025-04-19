#include "mizu/application.hpp"
#include "mizu/engine.hpp"
#include "mizu/payloads.hpp"

namespace mizu {
Application::Application(Engine *engine) : engine(engine) {
    callback_id = engine->callbacks.reg();

    engine->callbacks.sub<PUpdate>(callback_id, [&](const PUpdate &p) { update(p.dt); });
    engine->callbacks.sub<PDraw>(callback_id, [&](const PDraw &) { draw(); });
}
} // namespace mizu
