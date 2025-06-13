#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), g2d(engine->g2d.get()), input(engine->input.get()), window(engine->window.get()) {}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->pressed(mizu::Key::F1))
        g2d->set_vsync(!g2d->vsync());
}

void Ethereal::draw() {
    g2d->clear(mizu::rgba(0x00000000));

    g2d->triangle({50, 50}, {100, 100}, {50, 100}, mizu::rgba(0xffff0080));
    g2d->line({0, 0}, {input->mouse_x(), input->mouse_y()}, mizu::rgba(0xff00ff80));
    g2d->triangle({120, 120}, {180, 180}, {120, 180}, mizu::rgba(0x00ffff80));
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {500, 500}, [](auto &) {}).mainloop<Ethereal>();
}
