#ifndef MIZU_APPLICATION_HPP
#define MIZU_APPLICATION_HPP

namespace mizu {
class Engine;

class Application {
public:
    Engine *engine;

    Application(Engine *engine) : engine(engine) {}
    virtual ~Application() { engine = nullptr; }

    virtual void update(double dt) = 0;

    virtual void draw() = 0;
};
} // namespace mizu

#endif // MIZU_APPLICATION_HPP
