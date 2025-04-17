#ifndef MIZU_APPLICATION_HPP
#define MIZU_APPLICATION_HPP

#include "mizu/module.hpp"

namespace mizu {
class Engine;

class Application : public Module {
public:
    Engine *engine;

    explicit Application(Engine *engine) : engine(engine) {}
    ~Application() override { engine = nullptr; }

    std::string name() const override { return "mizu::Application"; }

    virtual void update(double dt) = 0;

    virtual void draw() = 0;
};
} // namespace mizu

#endif // MIZU_APPLICATION_HPP
