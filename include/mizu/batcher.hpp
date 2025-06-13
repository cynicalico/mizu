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
    std::size_t vertices_per_obj;

    std::unique_ptr<gloo::StaticSizeBuffer<float>> vbo;
    std::unique_ptr<gloo::VertexArray> vao;

    Batch(gloo::Context &gl, BatchType type, gloo::Shader *shader, std::size_t capacity, gloo::FillMode fill_mode);
};

struct BatchListBase {
    gloo::Context &gl;

    BatchType type;
    gloo::FillMode fill_mode;
    gloo::Shader *shader;
    std::size_t batch_capacity;

    std::size_t active_idx;
    std::vector<Batch> batches;

    std::size_t last_batch_count;
    Ticker<> check_batches;
    bool checking_unused_;
    MaxPeriod<std::size_t> batch_count_max;

    BatchListBase(
            gloo::Context &gl,
            BatchType type,
            gloo::FillMode fill_mode,
            gloo::Shader *shader,
            std::size_t batch_capacity
    )
        : gl(gl),
          type(type),
          fill_mode(fill_mode),
          shader(shader),
          batch_capacity(batch_capacity),
          active_idx(0),
          batches(),
          last_batch_count(0),
          check_batches(std::chrono::seconds(10)),
          checking_unused_(false),
          batch_count_max(std::chrono::seconds(10)) {}
};

struct OpaqueBatchList : BatchListBase {
    OpaqueBatchList(gloo::Context &gl, BatchType type, gloo::Shader *shader, std::size_t batch_capacity)
        : BatchListBase(gl, type, gloo::FillMode::BackToFront, shader, batch_capacity) {}

    void add(std::initializer_list<float> vertex_data);

    void draw(const glm::mat4 &projection);

    void clear();
};

struct TransBatchListSavedDrawParams {
    std::size_t list_idx;
    std::size_t batch_idx;
    std::size_t first;
    std::size_t count;
};

struct TransBatchList : BatchListBase {
    TransBatchList(gloo::Context &gl, BatchType type, gloo::Shader *shader, std::size_t batch_capacity)
        : BatchListBase(gl, type, gloo::FillMode::FrontToBack, shader, batch_capacity) {}

    std::size_t last_size{0};
    std::vector<TransBatchListSavedDrawParams> saved_draw_calls{};

    std::vector<TransBatchListSavedDrawParams> draw_calls();

    void add(std::initializer_list<float> vertex_data);

    void set_projection_and_sync(const glm::mat4 &projection);

    void draw(std::size_t batch_idx, std::size_t first, std::size_t count);

    void clear();

private:
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
    std::size_t last_trans_list_idx_{std::numeric_limits<std::size_t>::max()};
    std::vector<TransBatchListSavedDrawParams> trans_draw_calls_{};

    float z_level_{2.0f};

    void add_opaque_(BatchType type, std::initializer_list<float> vertex_data);

    void add_trans_(BatchType type, std::initializer_list<float> vertex_data);
    void push_saved_draw_calls_();
};
} // namespace mizu

#endif // MIZU_BATCHER_HPP
