#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::G2d &g2d;
    mizu::InputMgr &input;
    mizu::Window &window;

    std::unique_ptr<mizu::FtFont> font;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), g2d(*engine->g2d), input(*engine->input), window(*engine->window) {
    font = std::make_unique<mizu::FtFont>(g2d, "example/font/cozette.otb");
}

void Ethereal::update(double dt) {
    if (input.pressed(mizu::Key::Escape))
        engine->shutdown();
}

void Ethereal::draw() {
    g2d.clear(mizu::rgb(0x000000));

    font->draw("This text looks like a bitmap because it is, but it's from an .otb file!", 13, {50, 50});
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &b) { b.display(1); }).mainloop<Ethereal>();
}
