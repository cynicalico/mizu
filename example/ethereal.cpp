#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::G2d &g2d;
    mizu::InputMgr &input;
    mizu::Window &window;

    std::unique_ptr<mizu::Font> font;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;

    void key_release_callback(mizu::Key key, mizu::Mod mods) override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), g2d(*engine->g2d), input(*engine->input), window(*engine->window) {
    font = std::make_unique<mizu::Font>(g2d, "example/font/ter-u20b.bdf");
}

void Ethereal::update(double dt) {}

void Ethereal::draw() {
    g2d.clear(mizu::rgb(0x000000));

    const auto text = "Hello, world!\nMultiline text\nOptimized for bitmap fonts that I want right now";
    font->draw(text, {50, 50 + font->line_height()});
}

void Ethereal::key_release_callback(const mizu::Key key, mizu::Mod) {
    if (key == mizu::Key::Escape)
        engine->shutdown();
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &) {}).mainloop<Ethereal>();
}
