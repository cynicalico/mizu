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
    std::unique_ptr<gloo::Buffer<float>> vbo;
    GLuint vao;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine) : Application(engine), input(engine->input.get()), g2d(engine->g2d.get()) {
    shader = g2d->shader_builder()
                     .stage_src(gloo::ShaderType::Vertex, VERT_SRC)
                     .stage_src(gloo::ShaderType::Fragment, FRAG_SRC)
                     .link();
    if (!shader) engine->shutdown();

    vbo = g2d->buffer<float>(1024);
    vbo->push({-0.5f, -0.5f, 0.0f, /* */ 0.5f, -0.5f, 0.0f, /* */ 0.0f, 0.5f, 0.0f});

    engine->gl.ctx.GenVertexArrays(1, &vao);
    engine->gl.ctx.BindVertexArray(vao);
    vbo->bind(gloo::BufferTarget::Array);
    engine->gl.ctx.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(0));
    engine->gl.ctx.EnableVertexAttribArray(0);
    vbo->unbind(gloo::BufferTarget::Array);
    engine->gl.ctx.BindVertexArray(0);
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape)) engine->shutdown();

    vbo->sync_gl(gloo::BufferTarget::Array);
}

void Ethereal::draw() {
    g2d->clear(mizu::colorscheme::campbell::BLACK, mizu::ClearBit::Color | mizu::ClearBit::Depth);

    shader->use();
    engine->gl.ctx.BindVertexArray(vao);
    engine->gl.ctx.DrawArrays(GL_TRIANGLES, 0, 3);
    engine->gl.ctx.BindVertexArray(0);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
