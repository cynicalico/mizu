#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::AudioMgr *audio;
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine),
      audio(engine->audio.get()),
      g2d(engine->g2d.get()),
      input(engine->input.get()),
      window(engine->window.get()) {}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->pressed(mizu::Key::F1))
        g2d->set_vsync(!g2d->vsync());
}

void Ethereal::draw() {
    g2d->clear(mizu::rgba(0x00000000), mizu::ClearBit::Color | mizu::ClearBit::Depth);

    g2d->triangle({100, 100}, {200, 200}, {100, 200}, mizu::rgba(0xff000080));
    g2d->triangle({160, 200}, {260, 300}, {160, 300}, mizu::rgba(0x0000ff80));
    g2d->triangle({130, 150}, {230, 250}, {130, 250}, mizu::rgba(0x00ff0080));
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {500, 500}, [](auto &b) { b.display(1); }).mainloop<Ethereal>();
}
