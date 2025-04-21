#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::InputMgr *input;
    mizu::G2d *g2d;

    mizu::Rgba clear_color;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), input(engine->input.get()), g2d(engine->g2d.get()), clear_color(mizu::rgb(0xff0000)) {}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape)) engine->shutdown();

    clear_color = mizu::rgb(
            std::round(255.0 * (input->mouse_x() / (float)engine->window->get_size().w)),
            std::round(255.0 * (input->mouse_y() / (float)engine->window->get_size().h)),
            0
    );
}

void Ethereal::draw() {
    g2d->set_clear_color(clear_color);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
