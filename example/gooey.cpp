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

    explicit Gooey(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Gooey::Gooey(mizu::Engine *engine)
    : Application(engine), g2d(*engine->g2d), input(*engine->input), window(*engine->window) {
    font_tex = g2d.load_texture(
            "example/font/1px_7x9_no_bg.png", gloo::MinFilter::NearestMipmapNearest, gloo::MagFilter::Nearest);
    font = std::make_unique<mizu::CodePage437>(g2d, *font_tex, glm::uvec2{7, 9}, 2);

    root = std::make_unique<gui::VStack>(gui::Padding(5.0f), 5.0f);

    root->add_child<gui::Button>(font.get(), "Button 1", 2.0f);

    auto l1 = root->add_child<gui::HStack>(gui::Padding(5.0f), 5.0f);

    l1->add_child<gui::Button>(font.get(), "Button 2", 2.0f);

    auto l2 = l1->add_child<gui::VStack>(gui::Padding(5.0f), 5.0f);

    l2->add_child<gui::Button>(font.get(), "Button 3", 2.0f);

    l2->add_child<gui::Button>(font.get(), "Button 4", 2.0f);

    l1->add_child<gui::Button>(font.get(), "Button 5", 2.0f);

    l1->add_child<gui::Button>(font.get(), "Button 6", 2.0f);

    root->add_child<gui::Button>(font.get(), "Button 7", 2.0f);
}

void Gooey::update(double dt) {
    if (input.pressed(mizu::Key::Escape))
        engine->shutdown();
}

void Gooey::draw() {
    g2d.clear(mizu::rgb(0x000000));

    root->calc_size(window.size());
    root->draw(g2d, {(window.size().x - root->size.x) / 2, (window.size().y - root->size.y) / 2});
}

int main(int, char *[]) {
    mizu::Engine("gooey", {1280, 720}, [](auto &b) { b.resizable(); }).mainloop<Gooey>();
}
