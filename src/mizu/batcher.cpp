#include "mizu/batcher.hpp"
#include "mizu/io.hpp"

namespace mizu {
Batch::Batch(GladGLContext &gl, BatchType type, gloo::Shader *shader, std::size_t capacity) {
    switch (type) {
    case BatchType::Point:
        vertex_size = 6;
        vertices_per_obj = 1;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(gl, vertex_size * vertices_per_obj * capacity);
        vao = gloo::VertexArrayBuilder(gl)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 3)
                      .build();
        break;
    case BatchType::Line:
        vertex_size = 9;
        vertices_per_obj = 2;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(gl, vertex_size * vertices_per_obj * capacity);
        vao = gloo::VertexArrayBuilder(gl)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 3)
                      .vec("rot_params", 3)
                      .build();
        break;
    case BatchType::Triangle:
        vertex_size = 9;
        vertices_per_obj = 3;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(gl, vertex_size * vertices_per_obj * capacity);
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
    if (batches.empty() || batches.back().vbo->is_full())
        batches.emplace_back(gl, type, shader, capacity);
    batches.back().vbo->push(vertex_data);
}

void BatchList::draw(const glm::mat4 &projection) const {
    shader->use();
    shader->uniform("proj", projection);

    for (auto &batch: batches) {
        batch.vbo->sync_gl(gloo::BufferTarget::Array);

        gloo::DrawMode draw_mode;
        switch (type) {
        case BatchType::Point: draw_mode = gloo::DrawMode::Points; break;
        case BatchType::Line: draw_mode = gloo::DrawMode::Lines; break;
        case BatchType::Triangle: draw_mode = gloo::DrawMode::Triangles; break;
        }
        batch.vao->draw_arrays(
                draw_mode, batch.vbo->front() / batch.vertex_size, batch.vbo->size() / batch.vertex_size
        );
    }
}

void BatchList::clear() {
    batches.clear();
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
                      .type = BatchType::Point,
                      .shader = shaders_[0].get(),
                      .capacity = static_cast<std::size_t>(std::floor(8e6 / (32 * 6 * 1))),
                      .batches = std::vector<Batch>()
              },
              BatchList{
                      .gl = ctx_.ctx,
                      .type = BatchType::Line,
                      .shader = shaders_[1].get(),
                      .capacity = static_cast<std::size_t>(std::floor(8e6 / (32 * 9 * 2))),
                      .batches = std::vector<Batch>()
              },
              BatchList{
                      .gl = ctx_.ctx,
                      .type = BatchType::Triangle,
                      .shader = shaders_[2].get(),
                      .capacity = static_cast<std::size_t>(std::floor(8e6 / (32 * 9 * 3))),
                      .batches = std::vector<Batch>()
              }
      } {}

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
}
} // namespace mizu
