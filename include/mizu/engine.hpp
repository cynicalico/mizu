#ifndef MIZU_ENGINE_HPP
#define MIZU_ENGINE_HPP

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include "gloo/context.hpp"
#include "mizu/application.hpp"
#include "mizu/callback_mgr.hpp"
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

    Engine(const std::string& window_title, Size2d<int> window_size, WindowBuildFunc f);
    Engine(const std::string &window_title, WindowBuildFunc f);

    ~Engine();

    template<typename T>
        requires std::derived_from<T, Application>
    void mainloop();

private:
    bool running_;

    struct WindowBuilderParams_ {
        std::string window_title;
        Size2d<int> window_size;
        WindowBuildFunc build;
    };

    WindowBuilderParams_ window_builder_params_;
    void initialize_systems_();

    void poll_events_();
};

template<typename T>
    requires std::derived_from<T, Application>
void Engine::mainloop() {
    initialize_systems_();

    auto application = T(this);

    do {
        poll_events_();

        callbacks.pub_nowait<PPreUpdate>();
        callbacks.pub_nowait<PUpdate>(0.0);
        callbacks.pub_nowait<PPostUpdate>();

        callbacks.pub_nowait<PPreDraw>();
        callbacks.pub_nowait<PDraw>();
        callbacks.pub_nowait<PPostDraw>();

        callbacks.pub_nowait<PPresent>();
    } while (running_);
}
} // namespace mizu

#endif // MIZU_ENGINE_HPP
