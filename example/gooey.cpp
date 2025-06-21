#include "mizu/mizu.hpp"

namespace gui = mizu::gui;

class Gooey final : public mizu::Application {
public:
    mizu::G2d &g2d;
    mizu::InputMgr &input;
    mizu::Window &window;

    std::unique_ptr<mizu::Texture> font_tex;
    std::unique_ptr<mizu::CodePage437> font;

    std::unique_ptr<gui::Node> root;
    std::unique_ptr<gui::Node> l1;
    std::unique_ptr<gui::Node> l2;
    std::unique_ptr<gui::Node> button1;
    std::unique_ptr<gui::Node> button2;
    std::unique_ptr<gui::Node> button3;
    std::unique_ptr<gui::Node> button4;
    std::unique_ptr<gui::Node> button5;
    std::unique_ptr<gui::Node> button6;
    std::unique_ptr<gui::Node> button7;

    explicit Gooey(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Gooey::Gooey(mizu::Engine *engine)
    : Application(engine), g2d(*engine->g2d), input(*engine->input), window(*engine->window) {
    font_tex = g2d.load_texture(
            "example/font/1px_7x9_no_bg.png", gloo::MinFilter::NearestMipmapNearest, gloo::MagFilter::Nearest);
    font = std::make_unique<mizu::CodePage437>(g2d, *font_tex, glm::uvec2{7, 9}, 2);

    root = std::make_unique<gui::VStack>();

    button1 = std::make_unique<gui::Button>(font.get(), "Button 1", 2.0f);
    root->add_child(button1.get());

    l1 = std::make_unique<gui::HStack>();
    root->add_child(l1.get());

    button2 = std::make_unique<gui::Button>(font.get(), "Button 2", 2.0f);
    l1->add_child(button2.get());

    l2 = std::make_unique<gui::VStack>();
    l1->add_child(l2.get());

    button3 = std::make_unique<gui::Button>(font.get(), "Button 3", 2.0f);
    l2->add_child(button3.get());

    button4 = std::make_unique<gui::Button>(font.get(), "Button 4", 2.0f);
    l2->add_child(button4.get());

    button5 = std::make_unique<gui::Button>(font.get(), "Button 5", 2.0f);
    l1->add_child(button5.get());

    button6 = std::make_unique<gui::Button>(font.get(), "Button 6", 2.0f);
    l1->add_child(button6.get());

    button7 = std::make_unique<gui::Button>(font.get(), "Button 7", 2.0f);
    root->add_child(button7.get());
}

void Gooey::update(double dt) {
    if (input.pressed(mizu::Key::Escape))
        engine->shutdown();
}

void Gooey::draw() {
    g2d.clear(mizu::rgb(0x000000));

    root->calc_size(window.size());
    root->draw(g2d, {(window.size().x - root->size().x) / 2, (window.size().y - root->size().y) / 2});
}

int main(int, char *[]) {
    mizu::Engine("gooey", {1280, 720}, [](auto &b) { b.resizable(); }).mainloop<Gooey>();
}
