#include "mizu/mizu.hpp"

const auto VERT_SRC = R"glsl(
#version 330 core
in vec3 in_pos;
in vec3 in_color;

out vec3 color;

void main() {
    color = in_color;
    gl_Position = vec4(in_pos, 1.0);
}
)glsl";

const auto FRAG_SRC = R"glsl(
#version 330 core
in vec3 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0f);
}
)glsl";

class Ethereal final : public mizu::Application {
public:
    mizu::InputMgr *input;
    mizu::G2d *g2d;

    std::unique_ptr<gloo::Shader> shader;
    std::unique_ptr<gloo::StaticSizeBuffer<float>> vbo;
    std::unique_ptr<gloo::VertexArray> vao;

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

    vbo = g2d->buffer<float>(18 * 10);
    // clang-format off
    vbo->push({
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    });
    // clang-format on

    vao = g2d->vertex_array_builder()
                  .from(shader.get())
                  .from(vbo.get(), gloo::BufferTarget::Array)
                  .vec("in_pos", 3)
                  .vec("in_color", 3)
                  .build();
    if (!vao) engine->shutdown();
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape)) engine->shutdown();

    if (input->pressed(mizu::MouseButton::Left)) {
        if (!vbo->is_full()) {
            float x0 = mizu::rng::get<float>(-1.0, 1.0);
            float y0 = mizu::rng::get<float>(-1.0, 1.0);
            float x1 = mizu::rng::get<float>(-1.0, 1.0);
            float y1 = mizu::rng::get<float>(-1.0, 1.0);
            float x2 = mizu::rng::get<float>(-1.0, 1.0);
            float y2 = mizu::rng::get<float>(-1.0, 1.0);
            float r = mizu::rng::get<float>();
            float g = mizu::rng::get<float>();
            float b = mizu::rng::get<float>();
            // clang-format off
            vbo->push({
                x0, y0, 0.0f, r, g, b,
                x1, y1, 0.0f, r, g, b,
                x2, y2, 0.0f, r, g, b
            });
            // clang-format on
        } else {
            SPDLOG_INFO("Buffer is full");
        }
    }
}

void Ethereal::draw() {
    g2d->clear(mizu::colorscheme::campbell::BLACK, mizu::ClearBit::Color | mizu::ClearBit::Depth);

    vbo->sync_gl(gloo::BufferTarget::Array);
    shader->use();
    vao->draw_arrays(gloo::DrawMode::Triangles, vbo->front() / 6, vbo->size() / 6);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
