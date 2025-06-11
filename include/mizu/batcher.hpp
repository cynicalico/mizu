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

    Batch(GladGLContext &gl, BatchType type, gloo::Shader *shader, std::size_t capacity, gloo::FillMode fill_mode);
};

struct BatchList {
    GladGLContext &gl;

    BatchType type;
    gloo::FillMode fill_mode;
    gloo::Shader *shader;
    std::size_t batch_capacity;

    std::size_t active_idx{0};
    std::vector<Batch> batches{};

    std::size_t last_batch_count{0};
    Ticker<> check_batches{std::chrono::seconds(10)};
    bool checking_unused_{false};
    MaxPeriod<std::size_t> batch_count_max{std::chrono::seconds(10)};

    void add(std::initializer_list<float> vertex_data);

    void draw(const glm::mat4 &projection) const;

    void clear();
};

class Batcher {
public:
    explicit Batcher(gloo::Context &ctx);

    NO_COPY(Batcher)
    NO_MOVE(Batcher)

    float z();

    void add(BatchType type, bool trans, std::initializer_list<float> vertex_data);

    void draw(glm::mat4 projection) const;

    void clear();

private:
    gloo::Context &ctx_;

    std::unique_ptr<gloo::Shader> shaders_[3];
    BatchList opaque_batch_lists_[3];
    BatchList trans_batch_lists_[3];
    float z_level_{2.0f};

    void add_opaque_(BatchType type, std::initializer_list<float> vertex_data);
    void add_trans_(BatchType type, std::initializer_list<float> vertex_data);
};
} // namespace mizu

#endif // MIZU_BATCHER_HPP
