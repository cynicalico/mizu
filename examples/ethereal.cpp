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

    g2d->fill_rect({50, 50}, {50, 50}, {0, 0, 0}, mizu::rgb(0xffffff));
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &) {}).mainloop<Ethereal>();
}
