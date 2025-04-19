#ifndef MIZU_APPLICATION_HPP
#define MIZU_APPLICATION_HPP

#include <cstdlib>

namespace mizu {
class Engine;

class Application {
public:
    Engine *engine;

    explicit Application(Engine *engine);

    virtual ~Application() { engine = nullptr; }

    virtual void update(double dt) = 0;

    virtual void draw() = 0;

protected:
    std::size_t callback_id{};
};
} // namespace mizu

#endif // MIZU_APPLICATION_HPP
