#include <numbers>
#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::AudioMgr *audio;
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    std::vector<mizu::Triangle<>> triangles{};

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine),
      audio(engine->audio.get()),
      g2d(engine->g2d.get()),
      input(engine->input.get()),
      window(engine->window.get()) {}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->pressed(mizu::Key::F1))
        g2d->set_vsync(!g2d->vsync());

    if (input->pressed(mizu::MouseButton::Left)) {
        float cx = input->mouse_x();
        float cy = input->mouse_y();
        triangles.emplace_back(
                glm::tvec2<float>{
                        cx + 20.0f * std::cosf(std::numbers::pi_v<float> / 2.0f),
                        cy - 20.0f * std::sinf(std::numbers::pi_v<float> / 2.0f)
                },
                glm::tvec2<float>{
                        cx + 20.0f * std::cosf(7.0f * std::numbers::pi_v<float> / 6.0f),
                        cy - 20.0f * std::sinf(7.0f * std::numbers::pi_v<float> / 6.0f)
                },
                glm::tvec2<float>{
                        cx + 20.0f * std::cosf(11.0f * std::numbers::pi_v<float> / 6.0f),
                        cy - 20.0f * std::sinf(11.0f * std::numbers::pi_v<float> / 6.0f)
                },
                mizu::rgb_f(mizu::rng::get<float>(), mizu::rng::get<float>(), mizu::rng::get<float>())
        );
    }
}

void Ethereal::draw() {
    g2d->clear(mizu::rgb(0x000000), mizu::ClearBit::Color | mizu::ClearBit::Depth);

    for (const auto &triangle: triangles)
        g2d->triangle(triangle);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
