#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    std::unique_ptr<mizu::Texture> font_tex;
    std::unique_ptr<mizu::CodePage437> font;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), g2d(engine->g2d.get()), input(engine->input.get()), window(engine->window.get()) {
    font_tex = g2d->load_texture(
            "examples/font/1px_7x9.png", gloo::MinFilter::NearestMipmapNearest, gloo::MagFilter::Nearest
    );
    font = std::make_unique<mizu::CodePage437>(g2d, *font_tex, glm::uvec2{7, 9}, 2);
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->pressed(mizu::Key::F1))
        g2d->set_vsync(!g2d->vsync());
}

void Ethereal::draw() {
    g2d->clear(mizu::rgb(0x000000));

    font->draw("Hello, world!\nThis is text!", {50, 50}, 2.0f);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &) {}).mainloop<Ethereal>();
}
