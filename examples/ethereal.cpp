#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <numbers>
#include "mizu/mizu.hpp"

const auto VERT_SRC = R"glsl(
#version 330 core
in vec3 in_pos;
in vec3 in_color;
in vec3 in_rot;

out vec3 color;

uniform mat4 proj;

void main() {
    color = in_color;

    float c = cos(in_rot.z);
    float s = sin(in_rot.z);
    float xtr = -in_rot.x * c + in_rot.y * s + in_rot.x;
    float ytr = -in_rot.x * s - in_rot.y * c + in_rot.y;

    mat4 rot = mat4(
        vec4(c,   s,   0.0, 0.0),
        vec4(-s,  c,   0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(xtr, ytr, 0.0, 1.0)
    );

    gl_Position = proj * rot * vec4(in_pos, 1.0);
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

    mizu::Ticker<> ticker;

    explicit Ethereal(mizu::Engine *engine);

    void update(double dt) override;

    void draw() override;
};

Ethereal::Ethereal(mizu::Engine *engine)
    : Application(engine),
      window(engine->window.get()),
      input(engine->input.get()),
      g2d(engine->g2d.get()),
      proj(glm::identity<glm::mat4>()),
      ticker(std::chrono::milliseconds(1)) {
    shader = g2d->shader_builder()
                     .stage_src(gloo::ShaderType::Vertex, VERT_SRC)
                     .stage_src(gloo::ShaderType::Fragment, FRAG_SRC)
                     .link();

    vbo = g2d->buffer<float>(27 * 100'000);

    vao = g2d->vertex_array_builder()
                  .with(shader.get())
                  .with(vbo.get(), gloo::BufferTarget::Array)
                  .vec("in_pos", 3)
                  .vec("in_color", 3)
                  .vec("in_rot", 3)
                  .build();
}

void Ethereal::update(double dt) {
    if (input->pressed(mizu::Key::Escape))
        engine->shutdown();

    if (input->pressed(mizu::Key::F1))
        g2d->set_vsync(!g2d->vsync());

    if (input->pressed(mizu::Key::R))
        vbo->clear();

    if (ticker.tick() > 0) {
        if (!vbo->is_full()) {
            float cx = mizu::rng::get<float>(10.0f, static_cast<float>(window->get_size().w) - 10.0f);
            float cy = mizu::rng::get<float>(10.0f, static_cast<float>(window->get_size().h) - 10.0f);
            float x0 = cx + 20.0f * std::cosf(std::numbers::pi / 2.0f);
            float y0 = cy - 20.0f * std::sinf(std::numbers::pi / 2.0f);
            float x1 = cx + 20.0f * std::cosf(7.0f * std::numbers::pi / 6.0f);
            float y1 = cy - 20.0f * std::sinf(7.0f * std::numbers::pi / 6.0f);
            float x2 = cx + 20.0f * std::cosf(11.0f * std::numbers::pi / 6.0f);
            float y2 = cy - 20.0f * std::sinf(11.0f * std::numbers::pi / 6.0f);
            auto r = mizu::rng::get<float>();
            auto g = mizu::rng::get<float>();
            auto b = mizu::rng::get<float>();
            auto theta = mizu::rng::get<float>(0, 2 * std::numbers::pi);
            // clang-format off
            vbo->push({
                x0, y0, 0.0f, r, g, b, cx, cy, theta,
                x1, y1, 0.0f, r, g, b, cx, cy, theta,
                x2, y2, 0.0f, r, g, b, cx, cy, theta
            });
            // clang-format on
        } else {
            SPDLOG_INFO("Buffer is full");
        }
    }

    proj = glm::ortho(0.0, static_cast<double>(window->get_size().w), static_cast<double>(window->get_size().h), 0.0);
}

void Ethereal::draw() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    mizu::dear::begin("FPS", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration) && [&] {
        auto fps_str = fmt::format("FPS: {:.2f}{}", engine->frame_counter.fps(), g2d->vsync() ? " (vsync)" : "");
        ImGui::Text("%s", fps_str.c_str());
    };

    g2d->clear(mizu::colorscheme::campbell::BLACK, mizu::ClearBit::Color | mizu::ClearBit::Depth);

    vbo->sync_gl(gloo::BufferTarget::Array);

    shader->use();
    shader->uniform("proj", proj);

    vao->draw_arrays(gloo::DrawMode::Triangles, vbo->front() / 9, vbo->size() / 9);
}

int main(int, char *[]) {
    mizu::Engine("ethereal", mizu::Size2d(1280, 720), [](auto &) {}).mainloop<Ethereal>();
}
