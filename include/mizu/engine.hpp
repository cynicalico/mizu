#ifndef MIZU_ENGINE_HPP
#define MIZU_ENGINE_HPP

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include "gloo/context.hpp"
#include "mizu/application.hpp"
#include "mizu/callback_mgr.hpp"
#include "mizu/input_mgr.hpp"
#include "mizu/payloads.hpp"
#include "mizu/types.hpp"
#include "mizu/window.hpp"

namespace mizu {
class Engine {
    using WindowBuildFunc = std::function<void(WindowBuilder &)>;

public:
    gloo::GlContext gl{};
    CallbackMgr callbacks{};

    std::unique_ptr<Window> window{nullptr};
    std::unique_ptr<InputMgr> input{nullptr};

    Engine(const std::string &window_title, Size2d<int> window_size, WindowBuildFunc f);
    Engine(const std::string &window_title, WindowBuildFunc f);

    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&other) noexcept = delete;
    Engine &operator=(Engine &&other) = delete;

    template<typename T>
        requires std::derived_from<T, Application>
    void mainloop();

private:
    bool running_;
    std::size_t callback_id_{0};

    void poll_events_();

    void register_callbacks_();
    void unregister_callbacks_();
};

template<typename T>
    requires std::derived_from<T, Application>
void Engine::mainloop() {
    auto application = T(this);

    do {
        callbacks.pub_nowait<PPreUpdate>(0.0);
        callbacks.pub_nowait<PUpdate>(0.0);
        callbacks.pub_nowait<PPostUpdate>(0.0);

        callbacks.pub_nowait<PPreDraw>();
        callbacks.pub_nowait<PDraw>();
        callbacks.pub_nowait<PPostDraw>();

        callbacks.pub_nowait<PPresent>();

        poll_events_();
        callbacks.poll<PEventQuit>(callback_id_);
    } while (running_);
}
} // namespace mizu

#endif // MIZU_ENGINE_HPP
