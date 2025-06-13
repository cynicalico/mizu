#ifndef MIZU_APPLICATION_HPP
#define MIZU_APPLICATION_HPP

#include <cstdlib>

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

protected:
    std::size_t callback_id{0};

private:
    void register_callbacks_();
    void unregister_callbacks_();
};
} // namespace mizu

#endif // MIZU_APPLICATION_HPP
