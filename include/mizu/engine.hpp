#ifndef MIZU_ENGINE_HPP
#define MIZU_ENGINE_HPP

#include <functional>
#include <glad/gl.h>
#include <memory>
#include <optional>
#include <string>
#include "mizu/application.hpp"
#include "mizu/types.hpp"
#include "mizu/window.hpp"

namespace mizu {
class Engine {
    using window_build_f =
            std::function<std::invoke_result_t<decltype(&window_builder::build), window_builder>(window_builder &)>;

public:
    std::unique_ptr<Window> window{nullptr};
    GladGLContext gl;

    Engine(const std::string &window_title, Size2d<int> window_size, const window_build_f &f);
    Engine(const std::string &window_title, const window_build_f &f);

    ~Engine();

    template<typename T>
        requires std::derived_from<T, Application>
    void mainloop();

private:
    bool running_;

    bool is_running_() const;

    void poll_events_();
};

template<typename T>
    requires std::derived_from<T, Application>
void Engine::mainloop() {
    T application = T(this);

    do {
        poll_events_();

        application.update(0.0);
        application.draw();

        window->swap();
    } while (is_running_());
}
} // namespace mizu

#endif // MIZU_ENGINE_HPP
