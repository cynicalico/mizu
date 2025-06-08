#include "mizu/batcher.hpp"
#include "mizu/io.hpp"

namespace mizu {
Batch::Batch(GladGLContext &gl, BatchType type, gloo::Shader *shader, std::size_t capacity) {
    switch (type) {
    case BatchType::Points:
        vertex_size = 6;
        vertices_per_obj = 1;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
                gl, vertex_size * vertices_per_obj * capacity, gloo::FillMode::BackToFront
        );
        vao = gloo::VertexArrayBuilder(gl)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 3)
                      .build();
        break;
    case BatchType::Lines:
        vertex_size = 9;
        vertices_per_obj = 2;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
                gl, vertex_size * vertices_per_obj * capacity, gloo::FillMode::BackToFront
        );
        vao = gloo::VertexArrayBuilder(gl)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 3)
                      .vec("rot_params", 3)
                      .build();
        break;
    case BatchType::Triangles:
        vertex_size = 9;
        vertices_per_obj = 3;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
                gl, vertex_size * vertices_per_obj * capacity, gloo::FillMode::BackToFront
        );
        vao = gloo::VertexArrayBuilder(gl)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 3)
                      .vec("rot_params", 3)
                      .build();
        break;
    }
}

void BatchList::add(std::initializer_list<float> vertex_data) {
    if (batches.empty()) {
        batches.emplace_back(gl, type, shader, batch_capacity);
    } else if (batches[active_idx].vbo->is_full()) {
        active_idx++;
        if (active_idx >= batches.size())
            batches.emplace_back(gl, type, shader, batch_capacity);
    }

    assert(vertex_data.size() % batches[active_idx].vertex_size == 0);
    batches[active_idx].vbo->push(vertex_data);
}

void BatchList::draw(const glm::mat4 &projection) const {
    if (batches.empty())
        return;

    shader->use();
    shader->uniform("proj", projection);

    for (std::size_t i = active_idx + 1; i-- > 0;) {
        auto &batch = batches[i];

        batch.vbo->sync_gl(gloo::BufferTarget::Array);

        gloo::DrawMode draw_mode;
        switch (type) {
        case BatchType::Points: draw_mode = gloo::DrawMode::Points; break;
        case BatchType::Lines: draw_mode = gloo::DrawMode::Lines; break;
        case BatchType::Triangles: draw_mode = gloo::DrawMode::Triangles; break;
        }

        batch.vao->draw_arrays(
                draw_mode, batch.vbo->front() / batch.vertex_size, batch.vbo->size() / batch.vertex_size
        );
    }
}

void BatchList::clear() {
    for (auto &batch: batches)
        batch.vbo->clear();
    active_idx = 0;
}

Batcher::Batcher(gloo::Context &ctx)
    : ctx_(ctx),
      shaders_{
              gloo::ShaderBuilder(ctx_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/points.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/points.frag"))
                      .link(),
              gloo::ShaderBuilder(ctx_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/lines.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/lines.frag"))
                      .link(),
              gloo::ShaderBuilder(ctx_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/tris.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/tris.frag"))
                      .link(),
      },
      batch_lists_{
              BatchList{
                      .gl = ctx_.ctx,
                      .type = BatchType::Points,
                      .shader = shaders_[0].get(),
                      // .batch_capacity = static_cast<std::size_t>(std::floor(8e6 / (32 * 6 * 1))),
                      .batch_capacity = 10,
              },
              BatchList{
                      .gl = ctx_.ctx,
                      .type = BatchType::Lines,
                      .shader = shaders_[1].get(),
                      // .batch_capacity = static_cast<std::size_t>(std::floor(8e6 / (32 * 9 * 2))),
                      .batch_capacity = 2,
              },
              BatchList{
                      .gl = ctx_.ctx,
                      .type = BatchType::Triangles,
                      .shader = shaders_[2].get(),
                      // .batch_capacity = static_cast<std::size_t>(std::floor(8e6 / (32 * 9 * 3))),
                      .batch_capacity = 10,
              }
      } {}

float Batcher::z() {
    return z_level_++;
}

void Batcher::add(BatchType type, std::initializer_list<float> vertex_data) {
    batch_lists_[unwrap(type)].add(vertex_data);
}

void Batcher::draw(glm::mat4 projection) const {
    for (const auto &list: batch_lists_)
        list.draw(projection);
}

void Batcher::clear() {
    for (auto &list: batch_lists_)
        list.clear();
    z_level_ = 2.0f;
}
} // namespace mizu
