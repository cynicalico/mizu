#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <numbers>
#include "mizu/mizu.hpp"

const auto VERT_SRC = R"glsl(
#version 330 core
in vec3 in_pos;
in vec3 in_color;

out vec3 color;

uniform mat4 proj;

void main() {
    color = in_color;
    gl_Position = proj * vec4(in_pos, 1.0);
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
    mizu::Window *window;
    mizu::InputMgr *input;
    mizu::G2d *g2d;

    std::unique_ptr<gloo::Shader> shader;
    std::unique_ptr<gloo::StaticSizeBuffer<float>> vbo;
    std::unique_ptr<gloo::VertexArray> vao;

    glm::mat4 proj;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine),
      window(engine->window.get()),
      input(engine->input.get()),
      g2d(engine->g2d.get()),
      proj(glm::identity<glm::mat4>()) {
    shader = g2d->shader_builder()
                     .stage_src(gloo::ShaderType::Vertex, VERT_SRC)
                     .stage_src(gloo::ShaderType::Fragment, FRAG_SRC)
                     .link();
    if (!shader) engine->shutdown();

    vbo = g2d->buffer<float>(18 * 100);

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

    if (input->pressed(mizu::Key::R)) vbo->clear();

    if (input->pressed(mizu::MouseButton::Left)) {
        if (!vbo->is_full()) {
            float cx = input->mouse_x();
            float cy = input->mouse_y();
            float x0 = cx + 20.0f * std::cosf(std::numbers::pi / 2.0f);
            float y0 = cy - 20.0f * std::sinf(std::numbers::pi / 2.0f);
            float x1 = cx + 20.0f * std::cosf(7.0f * std::numbers::pi / 6.0f);
            float y1 = cy - 20.0f * std::sinf(7.0f * std::numbers::pi / 6.0f);
            float x2 = cx + 20.0f * std::cosf(11.0f * std::numbers::pi / 6.0f);
            float y2 = cy - 20.0f * std::sinf(11.0f * std::numbers::pi / 6.0f);
            auto r = mizu::rng::get<float>();
            auto g = mizu::rng::get<float>();
            auto b = mizu::rng::get<float>();
            vbo->push({x0, y0, 0.0f, r, g, b, x1, y1, 0.0f, r, g, b, x2, y2, 0.0f, r, g, b});
        } else {
            SPDLOG_INFO("Buffer is full");
        }
    }

    proj = glm::ortho(0.0, (double)window->get_size().w, (double)window->get_size().h, 0.0);
}

void Ethereal::draw() {
    g2d->clear(mizu::colorscheme::campbell::BLACK, mizu::ClearBit::Color | mizu::ClearBit::Depth);

    vbo->sync_gl(gloo::BufferTarget::Array);

    shader->use();
    shader->uniform("proj", proj);

    vao->draw_arrays(gloo::DrawMode::Triangles, vbo->front() / 6, vbo->size() / 6);

    ImGui::Begin("FPS");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::End();
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
