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

    for (auto &triangle: triangles)
        triangle.rot.z += 45.0f * static_cast<float>(dt);

    if (input->pressed(mizu::Key::R))
        triangles.clear();

    if (input->pressed(mizu::MouseButton::Left)) {
        float cx = input->mouse_x();
        float cy = input->mouse_y();
        float theta = mizu::rng::get(0.0f, 360.0f);
        triangles.emplace_back(
                glm::vec2{cx + 20.0f * std::cosf(1.5707963705062866f), cy - 20.0f * std::sinf(1.5707963705062866f)},
                glm::vec2{cx + 20.0f * std::cosf(3.665191411972046f), cy - 20.0f * std::sinf(3.665191411972046f)},
                glm::vec2{cx + 20.0f * std::cosf(5.759586334228516f), cy - 20.0f * std::sinf(5.759586334228516f)},
                glm::vec3{cx, cy, theta},
                mizu::rgb_f(mizu::rng::get<float>(), mizu::rng::get<float>(), mizu::rng::get<float>())
        );
    }
}

void Ethereal::draw() {
    g2d->clear(mizu::rgb(0x000000), mizu::ClearBit::Color | mizu::ClearBit::Depth);

    g2d->line({0, 0}, {input->mouse_x(), input->mouse_y()}, mizu::rgb(0xffffff));

    for (const auto &triangle: triangles)
        g2d->triangle(triangle);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &) {}).mainloop<Ethereal>();
}
