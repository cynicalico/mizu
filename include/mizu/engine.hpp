#ifndef MIZU_ENGINE_HPP
#define MIZU_ENGINE_HPP

#include <functional>
#include <memory>
#include <string>
#include "gloo/context.hpp"
#include "mizu/application.hpp"
#include "mizu/audio_mgr.hpp"
#include "mizu/callback_mgr.hpp"
#include "mizu/dear.hpp"
#include "mizu/g2d.hpp"
#include "mizu/input_mgr.hpp"
#include "mizu/payloads.hpp"
#include "mizu/time.hpp"
#include "mizu/types.hpp"
#include "mizu/window.hpp"

namespace mizu {
class Engine {
    using WindowBuildFunc = std::function<void(WindowBuilder &)>;

public:
    gloo::Context gl{};
    CallbackMgr callbacks{};

    std::unique_ptr<AudioMgr> audio{nullptr};
    std::unique_ptr<Dear> dear{nullptr};
    std::unique_ptr<G2d> g2d{nullptr};
    std::unique_ptr<InputMgr> input{nullptr};
    std::unique_ptr<Window> window{nullptr};

    FrameCounter<> frame_counter;

    Engine(const std::string &window_title, Size2d<int> window_size, WindowBuildFunc f);
    Engine(const std::string &window_title, WindowBuildFunc f);

    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&other) noexcept = delete;
    Engine &operator=(Engine &&other) = delete;

    void shutdown();

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
        frame_counter.update();
        const auto dt = as_secs_dt<decltype(frame_counter)>(frame_counter.dt());

        callbacks.pub_nowait<PPreUpdate>(dt);
        callbacks.pub_nowait<PUpdate>(dt);
        callbacks.pub_nowait<PPostUpdate>(dt);

        callbacks.pub_nowait<PPreDraw>();
        callbacks.pub_nowait<PPreDrawOverlay>();

        callbacks.pub_nowait<PDraw>();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        dear::begin("FPS", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration) && [&] {
            auto fps_str = fmt::format("FPS: {:.2f}{}", frame_counter.fps(), g2d->vsync() ? " (vsync)" : "");
            ImGui::Text("%s", fps_str.c_str());
        };

        callbacks.pub_nowait<PDrawOverlay>();
        callbacks.pub_nowait<PPostDraw>();

        callbacks.pub_nowait<PPresent>();

        poll_events_();
        callbacks.poll<PEventQuit>(callback_id_);
    } while (running_);
}
} // namespace mizu

#endif // MIZU_ENGINE_HPP
