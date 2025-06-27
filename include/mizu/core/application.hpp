#ifndef MIZU_APPLICATION_HPP
#define MIZU_APPLICATION_HPP

#include "mizu/core/input_mgr.hpp"

namespace mizu {
class Engine;

class Application {
public:
    Engine *engine;

    explicit Application(Engine *engine);

    virtual ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    Application(Application &&other) noexcept = delete;
    Application &operator=(Application &&other) = delete;

    virtual void update(double dt) = 0;

    virtual void draw() = 0;

    virtual void key_press_callback(Key key, Mod mods);
    virtual void key_release_callback(Key key, Mod mods);

    virtual void mouse_move_callback(float x, float y, float dx, float dy);
    virtual void mouse_scroll_callback(float x, float y);
    virtual void mouse_press_callback(MouseButton button, Mod mods);
    virtual void mouse_release_callback(MouseButton button, Mod mods);
    virtual void mouse_enter_callback();
    virtual void mouse_exit_callback();

    virtual void text_input_callback(const char *text);

private:
    std::size_t callback_id{0};

    void pre_update_();

    void register_callbacks_();
    void unregister_callbacks_();
};
} // namespace mizu

#endif // MIZU_APPLICATION_HPP
