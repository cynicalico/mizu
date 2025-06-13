#ifndef MIZU_BATCHER_HPP
#define MIZU_BATCHER_HPP

#include <glad/gl.h>
#include "gloo/buffer.hpp"
#include "gloo/context.hpp"
#include "gloo/vertex_array.hpp"
#include "mizu/time.hpp"

namespace mizu {
enum class BatchType : std::size_t { Points = 0, Lines = 1, Triangles = 2 };

struct Batch {
    std::size_t vertex_size;
    std::unique_ptr<gloo::StaticSizeBuffer<float>> vbo;
    std::unique_ptr<gloo::VertexArray> vao;

    Batch(gloo::Context &gl, BatchType type, gloo::Shader *shader, std::size_t capacity, gloo::FillMode fill_mode);
};

class BatchListBase {
protected:
    gloo::Context &gl_;
    BatchType type_;
    gloo::Shader *shader_;
    gloo::FillMode fill_mode_;

    std::size_t active_idx_;
    std::vector<Batch> batches_;

    bool checking_unused_;
    Ticker<> check_batches_timer_;
    MaxPeriod<std::size_t> batch_count_max_;
    std::size_t last_batch_count_;

    BatchListBase(gloo::Context &gl, BatchType type, gloo::Shader *shader, gloo::FillMode fill_mode)
        : gl_(gl),
          type_(type),
          shader_(shader),
          fill_mode_(fill_mode),
          active_idx_(0),
          batches_(),
          checking_unused_(false),
          check_batches_timer_(std::chrono::seconds(10)),
          batch_count_max_(std::chrono::seconds(10)),
          last_batch_count_(0) {}

    NO_COPY(BatchListBase)
    NO_MOVE(BatchListBase)

    void cleanup_unused_();
};

class OpaqueBatchList : BatchListBase {
public:
    OpaqueBatchList(gloo::Context &gl, BatchType type, gloo::Shader *shader)
        : BatchListBase(gl, type, shader, gloo::FillMode::BackToFront) {}

    NO_COPY(OpaqueBatchList)
    NO_MOVE(OpaqueBatchList)

    void add(std::initializer_list<float> vertex_data);

    void draw(const glm::mat4 &projection);

    void clear();
};

struct TransBatchListDrawParams {
    std::size_t batch_idx;
    std::size_t first;
    std::size_t count;
    std::size_t list_idx{0};
};

class TransBatchList : BatchListBase {
public:
    TransBatchList(gloo::Context &gl, BatchType type, gloo::Shader *shader)
        : BatchListBase(gl, type, shader, gloo::FillMode::FrontToBack) {}

    NO_COPY(TransBatchList)
    NO_MOVE(TransBatchList)

    std::vector<TransBatchListDrawParams> draw_calls();

    void add(std::initializer_list<float> vertex_data);

    void set_projection_and_sync(const glm::mat4 &projection);

    void draw(std::size_t batch_idx, std::size_t first, std::size_t count);

    void clear();

private:
    std::size_t last_draw_call_offset_{0};
    std::vector<TransBatchListDrawParams> saved_draw_calls_{};

    void save_draw_call_();
};

class Batcher {
public:
    explicit Batcher(gloo::Context &ctx);

    NO_COPY(Batcher)
    NO_MOVE(Batcher)

    float z();

    void add(BatchType type, bool trans, std::initializer_list<float> vertex_data);

    void draw(glm::mat4 projection);

    void clear();

private:
    gloo::Context &ctx_;

    std::unique_ptr<gloo::Shader> shaders_[3];

    OpaqueBatchList opaque_batch_lists_[3];

    TransBatchList trans_batch_lists_[3];
    std::size_t last_trans_batch_list_idx_{std::numeric_limits<std::size_t>::max()};
    std::vector<TransBatchListDrawParams> saved_trans_draw_calls_{};

    float z_level_{2.0f};

    void add_opaque_(BatchType type, std::initializer_list<float> vertex_data);

    void add_trans_(BatchType type, std::initializer_list<float> vertex_data);
    void flush_trans_draw_calls_();
};
} // namespace mizu

#endif // MIZU_BATCHER_HPP
