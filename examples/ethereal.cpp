#include <glad/gl.h>
#include "mizu/application.hpp"
#include "mizu/engine.hpp"
#include "mizu/log.hpp"
#include "mizu/window.hpp"

class Ethereal final : public mizu::Application {
public:
    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine) : Application(engine) {}

void Ethereal::update(double dt) {}

void Ethereal::draw() {
    engine->gl.ctx.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    engine->gl.ctx.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &builder) {
        return builder.build();
    }).mainloop<Ethereal>();
}
