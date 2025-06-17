#include "mizu/core/batcher.hpp"
#include "mizu/util/io.hpp"

namespace mizu {
constexpr std::size_t vertex_size_map[4] = {7, 10, 10, 12};

constexpr std::size_t vertices_per_obj_map[4] = {1, 2, 3, 6};

constexpr auto MB = static_cast<std::size_t>(8e6);
constexpr std::size_t batch_capacity_map[4] = {
        MB / (32 * vertex_size_map[unwrap(BatchType::Points)] * vertices_per_obj_map[unwrap(BatchType::Points)]),
        MB / (32 * vertex_size_map[unwrap(BatchType::Lines)] * vertices_per_obj_map[unwrap(BatchType::Lines)]),
        MB / (32 * vertex_size_map[unwrap(BatchType::Triangles)] * vertices_per_obj_map[unwrap(BatchType::Triangles)]),
        MB / (32 * vertex_size_map[unwrap(BatchType::Tex)] * vertices_per_obj_map[unwrap(BatchType::Tex)])};

constexpr gloo::DrawMode draw_mode_map[4] = {
        gloo::DrawMode::Points, gloo::DrawMode::Lines, gloo::DrawMode::Triangles, gloo::DrawMode::Triangles};

Batch::Batch(gloo::Context &gl, BatchType type, gloo::Shader *shader, std::size_t capacity, gloo::FillMode fill_mode) {
    vertex_size = vertex_size_map[unwrap(type)];
    vbo = std::make_unique<gloo::StaticSizeBuffer<float>>(
            gl.ctx, vertex_size * vertices_per_obj_map[unwrap(type)] * capacity, fill_mode);

    switch (type) {
    case BatchType::Points:
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .build();
        break;
    case BatchType::Lines:
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .vec("rot_params", 3)
                      .build();
        break;
    case BatchType::Triangles:
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .vec("rot_params", 3)
                      .build();
        break;
    case BatchType::Tex:
        vao = gloo::VertexArrayBuilder(gl.ctx)
                      .with(shader)
                      .with(vbo.get(), gloo::BufferTarget::Array)
                      .vec("pos", 3)
                      .vec("color", 4)
                      .vec("rot_params", 3)
                      .vec("tex_coord", 2)
                      .build();
        break;
    }
}

void BatchListBase::cleanup_unused_() {
    batch_count_max_.update(active_idx_ + 1);
    if (checking_unused_) {
        if (check_batches_timer_.tick() >= 1) {
            auto max_10s = batch_count_max_.value();
            while (max_10s < batches_.size())
                batches_.pop_back();

            if (max_10s <= 1)
                checking_unused_ = false;
        }
    } else if (last_batch_count_ <= 1 && batches_.size() > 1) {
        check_batches_timer_.reset();
        checking_unused_ = true;
    }
    last_batch_count_ = active_idx_ + 1;
}

void OpaqueBatchList::add(const std::initializer_list<float> vertex_data) {
    if (batches_.empty()) {
        batches_.emplace_back(gl_, type_, shader_, batch_capacity_map[unwrap(type_)], fill_mode_);
    } else if (!batches_[active_idx_].vbo->has_room_for(vertex_data.size())) {
        active_idx_++;
        if (active_idx_ >= batches_.size())
            batches_.emplace_back(gl_, type_, shader_, batch_capacity_map[unwrap(type_)], fill_mode_);
    }

    assert(vertex_data.size() % batches_[active_idx_].vertex_size == 0);
    batches_[active_idx_].vbo->push(vertex_data);
}

void OpaqueBatchList::draw(const glm::mat4 &projection) {
    if (batches_.empty())
        return;

    shader_->use();
    shader_->uniform("proj", projection);

    for (std::size_t i = active_idx_ + 1; i-- > 0;) {
        auto &batch = batches_[i];

        batch.vbo->sync_gl(gloo::BufferTarget::Array);
        batch.vao->draw_arrays(
                draw_mode_map[unwrap(type_)],
                batch.vbo->front() / batch.vertex_size,
                batch.vbo->size() / batch.vertex_size);
    }
}

void OpaqueBatchList::clear() {
    cleanup_unused_();

    for (auto &batch: batches_)
        batch.vbo->clear();

    active_idx_ = 0;
}

std::vector<TransBatchListDrawParams> TransBatchList::draw_calls() {
    save_draw_call_();
    std::vector ret(saved_draw_calls_);
    saved_draw_calls_.clear();

    return ret;
}

void TransBatchList::add(const std::initializer_list<float> vertex_data) {
    if (batches_.empty()) {
        batches_.emplace_back(gl_, type_, shader_, batch_capacity_map[unwrap(type_)], fill_mode_);
    } else if (batches_[active_idx_].vbo->is_full()) {
        save_draw_call_();
        last_draw_call_offset_ = 0;

        active_idx_++;
        if (active_idx_ >= batches_.size())
            batches_.emplace_back(gl_, type_, shader_, batch_capacity_map[unwrap(type_)], fill_mode_);
    }

    assert(vertex_data.size() % batches_[active_idx_].vertex_size == 0);
    batches_[active_idx_].vbo->push(vertex_data);
}

void TransBatchList::set_projection_and_sync(const glm::mat4 &projection) {
    if (batches_.empty())
        return;

    shader_->use();
    shader_->uniform("proj", projection);

    for (std::size_t i = active_idx_ + 1; i-- > 0;)
        batches_[i].vbo->sync_gl(gloo::BufferTarget::Array);
}

void TransBatchList::draw(std::size_t batch_idx, std::size_t first, std::size_t count) {
    shader_->use();

    batches_[batch_idx].vao->draw_arrays(
            draw_mode_map[unwrap(type_)],
            first / batches_[batch_idx].vertex_size,
            count / batches_[batch_idx].vertex_size);
}

void TransBatchList::clear() {
    cleanup_unused_();

    for (auto &batch: batches_)
        batch.vbo->clear();

    active_idx_ = 0;
    last_draw_call_offset_ = 0;
}

void TransBatchList::save_draw_call_() {
    saved_draw_calls_.emplace_back(
            active_idx_, last_draw_call_offset_, batches_[active_idx_].vbo->size() - last_draw_call_offset_);
    last_draw_call_offset_ = batches_[active_idx_].vbo->size();
}

Batcher::Batcher(gloo::Context &ctx)
    : gl_(ctx),
      shaders_{
              gloo::ShaderBuilder(gl_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/points.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/points.frag"))
                      .link(),
              gloo::ShaderBuilder(gl_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/lines.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/lines.frag"))
                      .link(),
              gloo::ShaderBuilder(gl_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/tris.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/tris.frag"))
                      .link(),
              gloo::ShaderBuilder(gl_.ctx)
                      .stage_src(gloo::ShaderType::Vertex, *read_file("res/shader/tex.vert"))
                      .stage_src(gloo::ShaderType::Fragment, *read_file("res/shader/tex.frag"))
                      .link(),
      },
      opaque_batch_lists_{
              OpaqueBatchList(gl_, BatchType::Points, shaders_[0].get()),
              OpaqueBatchList(gl_, BatchType::Lines, shaders_[1].get()),
              OpaqueBatchList(gl_, BatchType::Triangles, shaders_[2].get())},
      trans_batch_lists_{
              TransBatchList(gl_, BatchType::Points, shaders_[0].get()),
              TransBatchList(gl_, BatchType::Lines, shaders_[1].get()),
              TransBatchList(gl_, BatchType::Triangles, shaders_[2].get()),
              TransBatchList(gl_, BatchType::Tex, shaders_[3].get())} {}

float Batcher::z() {
    return z_level_++;
}

void Batcher::add(BatchType type, bool trans, GLuint texture_id, const std::initializer_list<float> vertex_data) {
    if (trans) {
        add_trans_(type, texture_id, vertex_data);
    } else {
        assert(type != BatchType::Tex);
        add_opaque_(type, vertex_data);
    }
}

void Batcher::draw(glm::mat4 projection) {
    // Grab any draw calls from the most recent trans batch list
    flush_trans_draw_calls_();

    for (auto &list: opaque_batch_lists_)
        list.draw(projection);

    gl_.depth_mask(false);
    gl_.enable(gloo::Capability::Blend);
    gl_.blend_func(gloo::BlendFunc::SrcAlpha, gloo::BlendFunc::OneMinusSrcAlpha);

    for (auto &list: trans_batch_lists_)
        list.set_projection_and_sync(projection);

    for (auto &params: saved_trans_draw_calls_) {
        if (params.texture_id != 0) {
            gl_.ctx.ActiveTexture(GL_TEXTURE0);
            CHECK_GL_ERROR(gl_.ctx, ActiveTexture);
            gl_.ctx.BindTexture(GL_TEXTURE_2D, params.texture_id);
            CHECK_GL_ERROR(gl_.ctx, ActiveTexture);
        }
        trans_batch_lists_[params.list_idx].draw(params.batch_idx, params.first, params.count);
        if (params.texture_id != 0) {
            gl_.ctx.BindTexture(GL_TEXTURE_2D, 0);
            CHECK_GL_ERROR(gl_.ctx, BindTexture);
        }
    }

    gl_.depth_mask(true);
    gl_.disable(gloo::Capability::Blend);
}

void Batcher::clear() {
    for (auto &list: opaque_batch_lists_)
        list.clear();

    for (auto &list: trans_batch_lists_)
        list.clear();
    last_trans_batch_list_idx_ = NO_LAST_IDX_;
    last_texture_id_ = NO_LAST_ID_;
    saved_trans_draw_calls_.clear();

    z_level_ = 2.0f;
}

void Batcher::add_opaque_(BatchType type, const std::initializer_list<float> vertex_data) {
    opaque_batch_lists_[unwrap(type)].add(vertex_data);
}

void Batcher::add_trans_(BatchType type, GLuint texture_id, const std::initializer_list<float> vertex_data) {
    if (last_trans_batch_list_idx_ != unwrap(type))
        flush_trans_draw_calls_();
    else if (last_texture_id_ != NO_LAST_ID_ && last_texture_id_ != texture_id)
        flush_trans_draw_calls_();

    trans_batch_lists_[unwrap(type)].add(vertex_data);
    last_trans_batch_list_idx_ = unwrap(type);

    if (texture_id != 0)
        last_texture_id_ = texture_id;
}

void Batcher::flush_trans_draw_calls_() {
    if (last_trans_batch_list_idx_ == NO_LAST_IDX_)
        return;

    auto new_draw_calls = trans_batch_lists_[last_trans_batch_list_idx_].draw_calls();
    for (auto &draw_call: new_draw_calls) {
        draw_call.list_idx = last_trans_batch_list_idx_;
        draw_call.texture_id = last_texture_id_;
    }

    saved_trans_draw_calls_.reserve(saved_trans_draw_calls_.size() + new_draw_calls.size());
    saved_trans_draw_calls_.insert(saved_trans_draw_calls_.end(), new_draw_calls.begin(), new_draw_calls.end());
}
} // namespace mizu
