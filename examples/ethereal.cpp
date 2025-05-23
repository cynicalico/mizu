#include "mizu/mizu.hpp"

class Ethereal final : public mizu::Application {
public:
    mizu::InputMgr *input;
    mizu::G2d *g2d;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine) : Application(engine), input(engine->input.get()), g2d(engine->g2d.get()) {
    g2d->shader_builder()
            .stage_src(gloo::ShaderType::Vertex, R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)glsl")
            .stage_src(gloo::ShaderType::Fragment, R"glsl(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)glsl")
            .link();
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape)) engine->shutdown();
}

void Ethereal::draw() {
    g2d->set_clear_color(mizu::colorscheme::campbell::BLACK);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
