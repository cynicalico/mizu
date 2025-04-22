#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::InputMgr *input;
    mizu::G2d *g2d;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine) : Application(engine), input(engine->input.get()), g2d(engine->g2d.get()) {
    if (auto surf = mizu::read_image_to_sdl_surface("res/icon/16x16.png"))
        engine->window->set_icon(surf);
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();
}

void Ethereal::draw() {}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
