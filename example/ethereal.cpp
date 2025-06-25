#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::G2d &g2d;
    mizu::InputMgr &input;
    mizu::Window &window;

    std::unique_ptr<mizu::Ttf> font;

    std::string text;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), g2d(*engine->g2d), input(*engine->input), window(*engine->window) {
    font = std::make_unique<mizu::Ttf>(g2d, "example/font/IBMPlexSans-Regular.ttf");
    text = mizu::rng::base58(11);
}

void Ethereal::update(double dt) {
    if (input.pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input.pressed(mizu::MouseButton::Left))
        text = mizu::rng::base58(11);
}

void Ethereal::draw() {
    g2d.clear(mizu::rgb(0x000000));

    font->draw(text, {50, 50});
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &b) { b.display(1); }).mainloop<Ethereal>();
}
