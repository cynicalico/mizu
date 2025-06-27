#ifndef MIZU_ENGINE_HPP
#define MIZU_ENGINE_HPP

#include <functional>
#include <memory>
#include <string>
#include "gloo/context.hpp"
#include "mizu/core/application.hpp"
#include "mizu/core/audio_mgr.hpp"
#include "mizu/core/callback_mgr.hpp"
#include "mizu/core/dear.hpp"
#include "mizu/core/g2d.hpp"
#include "mizu/core/input_mgr.hpp"
#include "mizu/core/payloads.hpp"
#include "mizu/core/window.hpp"
#include "mizu/util/memusage.hpp"
#include "mizu/util/time.hpp"

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

    Engine(const std::string &window_title, glm::ivec2 window_size, WindowBuildFunc f);
    Engine(const std::string &window_title, WindowBuildFunc f);

    ~Engine();

    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&other) noexcept = delete;
    Engine &operator=(Engine &&other) = delete;

    void shutdown();

    bool show_fps() const;
    void set_show_fps(bool v);

    template<typename T, typename... Args>
        requires std::derived_from<T, Application>
    void mainloop(Args &&...args);

private:
    bool running_;
    bool show_fps_;
    std::size_t callback_id_{0};

    void poll_events_();

    void register_callbacks_();
    void unregister_callbacks_();
};

template<typename T, typename... Args>
    requires std::derived_from<T, Application>
void Engine::mainloop(Args &&...args) {
    auto application = T(this, std::forward<Args>(args)...);

    do {
        frame_counter.update();
        const auto dt = as_secs_dt<decltype(frame_counter)>(frame_counter.dt());

        callbacks.pub_nowait<PPreUpdate>(dt);
        callbacks.pub_nowait<PUpdate>(dt);
        callbacks.pub_nowait<PPostUpdate>(dt);

        callbacks.pub_nowait<PPreDraw>();
        callbacks.pub_nowait<PPreDrawOverlay>();

        if (show_fps_) {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
            dear::begin("FPS", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration) && [&] {
                const auto fps_str = fmt::format(
                        std::locale("en_US.UTF-8"),
                        "FPS: {:.2Lf}{} | Mem: {:.2Lf} MB",
                        frame_counter.fps(),
                        g2d->vsync() ? " (vsync)" : "",
                        memusage());
                ImGui::Text("%s", fps_str.c_str());
            };
            ImGui::PopStyleVar();
        }

        callbacks.pub_nowait<PDraw>();

        callbacks.pub_nowait<PPostDraw>();
        callbacks.pub_nowait<PDrawOverlay>();

        callbacks.pub_nowait<PPresent>();

        poll_events_();
        callbacks.poll<PEventQuit>(callback_id_);
    } while (running_);
}
} // namespace mizu

#endif // MIZU_ENGINE_HPP
