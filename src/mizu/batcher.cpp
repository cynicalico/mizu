#include "mizu/batcher.hpp"
#include "mizu/io.hpp"

namespace mizu {
gloo::DrawMode batch_type_to_draw_mode(BatchType type) {
    switch (type) {
    case BatchType::Points: return gloo::DrawMode::Points;
    case BatchType::Lines: return gloo::DrawMode::Lines;
    case BatchType::Triangles: return gloo::DrawMode::Triangles;
    default: std::unreachable();
    }
}

Batch::Batch(gloo::Context &gl, BatchType type, gloo::Shader *shader, std::size_t capacity, gloo::FillMode fill_mode) {
    switch (type) {
    case BatchType::Points:
        vertex_size = 7;
        vertices_per_obj = 1;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
                gl.ctx, vertex_size * vertices_per_obj * capacity, fill_mode
        );
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .build();
        break;
    case BatchType::Lines:
        vertex_size = 10;
        vertices_per_obj = 2;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
                gl.ctx, vertex_size * vertices_per_obj * capacity, fill_mode
        );
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .vec("rot_params", 3)
                      .build();
        break;
    case BatchType::Triangles:
        vertex_size = 10;
        vertices_per_obj = 3;
        vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
                gl.ctx, vertex_size * vertices_per_obj * capacity, fill_mode
        );
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .vec("rot_params", 3)
                      .build();
        break;
    }
}

void OpaqueBatchList::add(const std::initializer_list<float> vertex_data) {
    if (batches.empty()) {
        batches.emplace_back(gl, type, shader, batch_capacity, fill_mode);
    } else if (batches[active_idx].vbo->is_full()) {
        active_idx++;
        if (active_idx >= batches.size())
            batches.emplace_back(gl, type, shader, batch_capacity, fill_mode);
    }

    assert(vertex_data.size() % batches[active_idx].vertex_size == 0);
    batches[active_idx].vbo->push(vertex_data);
}

void OpaqueBatchList::draw(const glm::mat4 &projection) {
    if (batches.empty())
        return;

    shader->use();
    shader->uniform("proj", projection);

    for (std::size_t i = active_idx + 1; i-- > 0;) {
        auto &batch = batches[i];

        batch.vbo->sync_gl(gloo::BufferTarget::Array);
        batch.vao->draw_arrays(
                batch_type_to_draw_mode(type),
                batch.vbo->front() / batch.vertex_size,
                batch.vbo->size() / batch.vertex_size
        );
    }
}

void OpaqueBatchList::clear() {
    batch_count_max.update(active_idx + 1);

    if (checking_unused_) {
        if (check_batches.tick() >= 1) {
            auto max_10s = batch_count_max.value();
            while (max_10s < batches.size())
                batches.pop_back();

            if (max_10s <= 1)
                checking_unused_ = false;
        }
    } else if (last_batch_count <= 1 && batches.size() > 1) {
        check_batches.reset();
        checking_unused_ = true;
    }
    last_batch_count = active_idx + 1;

    for (auto &batch: batches)
        batch.vbo->clear();
    active_idx = 0;
}

std::vector<TransBatchListSavedDrawParams> TransBatchList::draw_calls() {
    save_draw_call_();
    std::vector ret(saved_draw_calls);
    saved_draw_calls.clear();

    return ret;
}

void TransBatchList::add(const std::initializer_list<float> vertex_data) {
    if (batches.empty()) {
        batches.emplace_back(gl, type, shader, batch_capacity, fill_mode);
    } else if (batches[active_idx].vbo->is_full()) {
        save_draw_call_();
        last_size = 0;

        active_idx++;
        if (active_idx >= batches.size())
            batches.emplace_back(gl, type, shader, batch_capacity, fill_mode);
    }

    assert(vertex_data.size() % batches[active_idx].vertex_size == 0);
    batches[active_idx].vbo->push(vertex_data);
}

void TransBatchList::set_projection_and_sync(const glm::mat4 &projection) {
    if (batches.empty())
        return;

    shader->use();
    shader->uniform("proj", projection);

    for (std::size_t i = active_idx + 1; i-- > 0;)
        batches[i].vbo->sync_gl(gloo::BufferTarget::Array);
}

void TransBatchList::draw(std::size_t batch_idx, std::size_t first, std::size_t count) {
    shader->use();

    batches[batch_idx].vao->draw_arrays(
            batch_type_to_draw_mode(type),
            first / batches[batch_idx].vertex_size,
            count / batches[batch_idx].vertex_size
    );
}

void TransBatchList::clear() {
    batch_count_max.update(active_idx + 1);

    if (checking_unused_) {
        if (check_batches.tick() >= 1) {
            auto max_10s = batch_count_max.value();
            while (max_10s < batches.size())
                batches.pop_back();

            if (max_10s <= 1)
                checking_unused_ = false;
        }
    } else if (last_batch_count <= 1 && batches.size() > 1) {
        check_batches.reset();
        checking_unused_ = true;
    }
    last_batch_count = active_idx + 1;

    for (auto &batch: batches)
        batch.vbo->clear();
    active_idx = 0;
    last_size = 0;
}

void TransBatchList::save_draw_call_() {
    saved_draw_calls.emplace_back(0, active_idx, last_size, batches[active_idx].vbo->size() - last_size);
    last_size = batches[active_idx].vbo->size();
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
      opaque_batch_lists_{
              OpaqueBatchList(
                      ctx_,
                      BatchType::Points,
                      shaders_[0].get(),
                      // static_cast<std::size_t>(std::floor(8e6 / (32 * 7 * 1)))
                      5
              ),
              OpaqueBatchList(
                      ctx_,
                      BatchType::Lines,
                      shaders_[1].get(),
                      // static_cast<std::size_t>(std::floor(8e6 / (32 * 10 * 2)))
                      5
              ),
              OpaqueBatchList(
                      ctx_,
                      BatchType::Triangles,
                      shaders_[2].get(),
                      // static_cast<std::size_t>(std::floor(8e6 / (32 * 10 * 3)))
                      5
              )
      },
      trans_batch_lists_{
              TransBatchList(
                      ctx_,
                      BatchType::Points,
                      shaders_[0].get(),
                      // static_cast<std::size_t>(std::floor(8e6 / (32 * 7 * 1)))
                      5
              ),
              TransBatchList(
                      ctx_,
                      BatchType::Lines,
                      shaders_[1].get(),
                      // static_cast<std::size_t>(std::floor(8e6 / (32 * 10 * 2)))
                      5
              ),
              TransBatchList(
                      ctx_,
                      BatchType::Triangles,
                      shaders_[2].get(),
                      // static_cast<std::size_t>(std::floor(8e6 / (32 * 10 * 3)))
                      5
              )
      } {}

float Batcher::z() {
    return z_level_++;
}

void Batcher::add(BatchType type, bool trans, const std::initializer_list<float> vertex_data) {
    if (trans)
        add_trans_(type, vertex_data);
    else
        add_opaque_(type, vertex_data);
}

void Batcher::draw(glm::mat4 projection) {
    if (last_trans_list_idx_ != std::numeric_limits<std::size_t>::max())
        push_saved_draw_calls_();

    for (auto &list: opaque_batch_lists_)
        list.draw(projection);

    ctx_.depth_mask(false);
    ctx_.enable(gloo::Capability::Blend);
    ctx_.blend_func(gloo::BlendFunc::SrcAlpha, gloo::BlendFunc::OneMinusSrcAlpha);

    for (auto &list: trans_batch_lists_)
        list.set_projection_and_sync(projection);

    for (auto &params: trans_draw_calls_)
        trans_batch_lists_[params.list_idx].draw(params.batch_idx, params.first, params.count);

    ctx_.depth_mask(true);
    ctx_.disable(gloo::Capability::Blend);
}

void Batcher::clear() {
    for (auto &list: opaque_batch_lists_)
        list.clear();

    for (auto &list: trans_batch_lists_)
        list.clear();
    last_trans_list_idx_ = std::numeric_limits<std::size_t>::max();
    trans_draw_calls_.clear();

    z_level_ = 2.0f;
}

void Batcher::add_opaque_(BatchType type, const std::initializer_list<float> vertex_data) {
    opaque_batch_lists_[unwrap(type)].add(vertex_data);
}

void Batcher::add_trans_(BatchType type, const std::initializer_list<float> vertex_data) {
    if (last_trans_list_idx_ != std::numeric_limits<std::size_t>::max() && last_trans_list_idx_ != unwrap(type))
        push_saved_draw_calls_();

    trans_batch_lists_[unwrap(type)].add(vertex_data);
    last_trans_list_idx_ = unwrap(type);
}

void Batcher::push_saved_draw_calls_() {
    auto new_draw_calls = trans_batch_lists_[last_trans_list_idx_].draw_calls();
    for (auto &draw_call: new_draw_calls)
        draw_call.list_idx = last_trans_list_idx_;

    trans_draw_calls_.reserve(trans_draw_calls_.size() + new_draw_calls.size());
    trans_draw_calls_.insert(trans_draw_calls_.end(), new_draw_calls.begin(), new_draw_calls.end());
}
} // namespace mizu
