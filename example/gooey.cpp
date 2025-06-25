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
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 1",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 1 clicked! id={}", b->id); }})
                  .start<mgui::HStack>({.outer_pad = mgui::Padding(0.0f), .inner_pad = 5.0f})
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 2",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 2 clicked! id={}", b->id); }})
                  .start<mgui::VStack>({.outer_pad = mgui::Padding(0.0f), .inner_pad = 5.0f})
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 3",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 3 clicked! id={}", b->id); }})
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 4",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 4 clicked! id={}", b->id); }})
                  .end()
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 5",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 5 clicked! id={}", b->id); }})
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 6",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 6 clicked! id={}", b->id); }})
                  .end()
                  .add<mgui::Button>(
                          {.font = font.get(),
                           .text = "Button 7",
                           .text_scale = 2.0f,
                           .onclick = [&](auto *b) { MIZU_LOG_INFO("Button 7 clicked! id={}", b->id); }})
                  .build();
}

void Gooey::update(double dt) {
    if (input.pressed(mizu::Key::Escape))
        engine->shutdown();

    gui->resize(window.size(), {0, 0});
    gui->update(input);
}

void Gooey::draw() {
    g2d.clear(mizu::rgb(0x000000));

    gui->draw(g2d);
}

int main(int, char *[]) {
    mizu::Engine("gooey", {1280, 720}, [](auto &b) { b.display(1).resizable(); }).mainloop<Gooey>();
}
