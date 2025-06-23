#include "mizu/mizu.hpp"

namespace mgui = mizu::gui;

class Gooey final : public mizu::Application {
public:
    mizu::G2d &g2d;
    mizu::InputMgr &input;
    mizu::Window &window;

    std::unique_ptr<mizu::Texture> font_tex;
    std::unique_ptr<mizu::CodePage437> font;

    std::unique_ptr<mgui::Gui> gui;

    explicit Gooey(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Gooey::Gooey(mizu::Engine *engine)
    : Application(engine), g2d(*engine->g2d), input(*engine->input), window(*engine->window) {
    font_tex = g2d.load_texture(
            "example/font/1px_7x9_no_bg.png", gloo::MinFilter::NearestMipmapNearest, gloo::MagFilter::Nearest);
    font = std::make_unique<mizu::CodePage437>(g2d, *font_tex, glm::uvec2{7, 9}, 2);

    gui = mgui::GuiBuilder()
                  .start<mgui::VStack>(
                          {.border = mgui::PxBorder{mizu::rgb(0xffffff)},
                           .outer_pad = mgui::Padding(5.0f),
                           .inner_pad = 5.0f})
                  .add<mgui::Button>({.font = font.get(), .text = "Button 1", .text_scale = 2.0f})
                  .start<mgui::HStack>({.outer_pad = mgui::Padding(0.0f), .inner_pad = 5.0f})
                  .add<mgui::Button>({.font = font.get(), .text = "Button 2", .text_scale = 2.0f})
                  .start<mgui::VStack>({.outer_pad = mgui::Padding(0.0f), .inner_pad = 5.0f})
                  .add<mgui::Button>({.font = font.get(), .text = "Button 3", .text_scale = 2.0f})
                  .add<mgui::Button>({.font = font.get(), .text = "Button 4", .text_scale = 2.0f})
                  .end()
                  .add<mgui::Button>({.font = font.get(), .text = "Button 5", .text_scale = 2.0f})
                  .add<mgui::Button>({.font = font.get(), .text = "Button 6", .text_scale = 2.0f})
                  .end()
                  .add<mgui::Button>({.font = font.get(), .text = "Button 7", .text_scale = 2.0f})
                  .build();
}

void Gooey::update(double dt) {
    if (input.pressed(mizu::Key::Escape))
        engine->shutdown();
}

void Gooey::draw() {
    g2d.clear(mizu::rgb(0x000000));

    gui->calc_size(window.size());
    gui->draw(g2d, {(window.size().x - gui->size().x) / 2, (window.size().y - gui->size().y) / 2});
}

int main(int, char *[]) {
    mizu::Engine("gooey", {1280, 720}, [](auto &b) { b.resizable(); }).mainloop<Gooey>();
}
