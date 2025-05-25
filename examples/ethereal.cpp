#include "mizu/mizu.hpp"

const auto VERT_SRC = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)glsl";

const auto FRAG_SRC = R"glsl(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)glsl";

class Ethereal final : public mizu::Application {
public:
    mizu::InputMgr *input;
    mizu::G2d *g2d;

    std::unique_ptr<gloo::Shader> shader;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine) : Application(engine), input(engine->input.get()), g2d(engine->g2d.get()) {
    auto opt = g2d->shader_builder()
                       .stage_src(gloo::ShaderType::Vertex, VERT_SRC)
                       .stage_src(gloo::ShaderType::Fragment, FRAG_SRC)
                       .link();
    if (opt) shader = std::make_unique<gloo::Shader>(std::move(*opt));

    g2d->set_clear_color(mizu::colorscheme::campbell::BLACK);
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape)) engine->shutdown();
}

void Ethereal::draw() {
    shader->use();
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
