#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    std::unique_ptr<mizu::Texture> arbok;
    std::unique_ptr<mizu::Texture> milotic;
    std::unique_ptr<mizu::Texture> serperior;
    std::unique_ptr<mizu::Texture> seviper;
    float theta{0.0f};

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine), g2d(engine->g2d.get()), input(engine->input.get()), window(engine->window.get()) {
    arbok = g2d->load_texture("examples/img/arbok.png", gloo::Scaling::Retro);
    milotic = g2d->load_texture("examples/img/milotic-f.png", gloo::Scaling::Retro);
    serperior = g2d->load_texture("examples/img/serperior.png", gloo::Scaling::Retro);
    seviper = g2d->load_texture("examples/img/seviper.png", gloo::Scaling::Retro);
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->pressed(mizu::Key::F1))
        g2d->set_vsync(!g2d->vsync());

    theta += 20.0f * dt;
}

void Ethereal::draw() {
    g2d->clear(mizu::rgba(0x00000000));

    const auto center = glm::vec2(window->size().x / 2.0f, window->size().y / 2.0f);

    const auto arbok_center = glm::vec2{
            center.x + window->size().x / 3.5f * std::sin(glm::radians(theta)),
            center.y - window->size().y / 3.5f * std::cos(glm::radians(theta))
    };
    g2d->texture(*arbok, {arbok_center.x - arbok->width() / 2, arbok_center.y - arbok->height() / 2}, {0, 0, 0});

    g2d->line({0, 0}, center, mizu::rgba(0xff00ff80));

    const auto milotic_center = glm::vec2{
            center.x + window->size().x / 3.5f * std::sin(glm::radians(theta + 90.0f)),
            center.y - window->size().y / 3.5f * std::cos(glm::radians(theta + 90.0f))
    };
    g2d->texture(
            *milotic, {milotic_center.x - milotic->width() / 2, milotic_center.y - milotic->height() / 2}, {0, 0, 0}
    );

    g2d->line({window->size().x, 0}, center, mizu::rgba(0xff00ff80));

    const auto serperior_center = glm::vec2{
            center.x + window->size().x / 3.5f * std::sin(glm::radians(theta + 180.0f)),
            center.y - window->size().y / 3.5f * std::cos(glm::radians(theta + 180.0f))
    };
    g2d->texture(
            *serperior,
            {serperior_center.x - serperior->width() / 2, serperior_center.y - serperior->height() / 2},
            {0, 0, 0}
    );

    g2d->line({0, window->size().y}, center, mizu::rgba(0xff00ff80));

    const auto seviper_center = glm::vec2{
            center.x + window->size().x / 3.5f * std::sin(glm::radians(theta + 270.0f)),
            center.y - window->size().y / 3.5f * std::cos(glm::radians(theta + 270.0f))
    };
    g2d->texture(
            *seviper, {seviper_center.x - seviper->width() / 2, seviper_center.y - seviper->height() / 2}, {0, 0, 0}
    );

    g2d->line({window->size().x, window->size().y}, center, mizu::rgba(0xff00ff80));
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {300, 300}, [](auto &) {}).mainloop<Ethereal>();
}
