#include <numbers>
#include "mizu/mizu.hpp"

struct RotTriangle {
    mizu::Triangle<> t;
    float r_mult;
};

class Ethereal final : public mizu::Application {
public:
    mizu::AudioMgr *audio;
    mizu::G2d *g2d;
    mizu::InputMgr *input;
    mizu::Window *window;

    mizu::Ticker<> repeat_triangle{std::chrono::milliseconds(100)};
    std::vector<RotTriangle> triangles{};

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;

    void push_triangle();
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
        triangle.t.rot.z += 45.0f * triangle.r_mult * static_cast<float>(dt);

    if (input->pressed(mizu::Key::R))
        triangles.clear();

    if (input->pressed(mizu::Key::F))
        triangles.erase(triangles.begin() + mizu::rng::get<std::size_t>(0, triangles.size() - 1));

    if (input->pressed(mizu::MouseButton::Left)) {
        push_triangle();
        repeat_triangle.reset();
    } else if (input->down(mizu::MouseButton::Left) && repeat_triangle.tick() >= 1) {
        push_triangle();
    }
}

void Ethereal::draw() {
    mizu::dear::begin("isoindfsdf") && [&] { ImGui::Text("%s", fmt::format("{}", triangles.size()).c_str()); };

    g2d->clear(mizu::rgb(0x000000), mizu::ClearBit::Color | mizu::ClearBit::Depth);

    for (const auto &triangle: triangles)
        g2d->triangle(triangle.t);
}

void Ethereal::push_triangle() {
    float cx = input->mouse_x();
    float cy = input->mouse_y();
    float theta = mizu::rng::get(0.0f, 360.0f);
    float r_mult = mizu::rng::get(-20.0f, 20.0f);
    triangles.emplace_back(
            mizu::Triangle{
                    glm::vec2{cx + 50.0f * std::cosf(1.5707963705062866f), cy - 50.0f * std::sinf(1.5707963705062866f)},
                    glm::vec2{cx + 50.0f * std::cosf(3.665191411972046f), cy - 50.0f * std::sinf(3.665191411972046f)},
                    glm::vec2{cx + 50.0f * std::cosf(5.759586334228516f), cy - 50.0f * std::sinf(5.759586334228516f)},
                    glm::vec3{cx, cy, theta},
                    mizu::rgb_f(mizu::rng::get<float>(), mizu::rng::get<float>(), mizu::rng::get<float>())
            },
            r_mult
    );
}

int main(int, char *[]) {
    mizu::Engine("ethereal", {1280, 720}, [](auto &) {}).mainloop<Ethereal>();
}
