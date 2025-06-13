#ifndef GLOO_VERTEX_ARRAY_HPP
#define GLOO_VERTEX_ARRAY_HPP

#include <glad/gl.h>
#include <memory>
#include "gloo/buffer.hpp"
#include "gloo/shader.hpp"
#include "mizu/util/class_helpers.hpp"
#include "mizu/util/is_any_of.hpp"

namespace gloo {
enum class DrawMode : GLenum {
    Points = GL_POINTS,
    Lines = GL_LINES,
    Triangles = GL_TRIANGLES,
};

class VertexArray {
    friend class VertexArrayBuilder;

public:
    GLuint id;

    ~VertexArray();

    NO_COPY(VertexArray);

    MOVE_CONSTRUCTOR(VertexArray);
    MOVE_ASSIGN_OP(VertexArray);

    void bind();
    void unbind();

    void draw_arrays(DrawMode mode, std::size_t first, std::size_t count);

private:
    GladGLContext &gl_;

    VertexArray(GladGLContext &gl, GLuint id);
};

class VertexArrayBuilder {
public:
    explicit VertexArrayBuilder(GladGLContext &gl);

    NO_COPY(VertexArrayBuilder)
    NO_MOVE(VertexArrayBuilder)

    VertexArrayBuilder &with(Shader *shader);

    template<typename T>
        requires mizu::IsAnyOf<T, float, int, unsigned int>
    VertexArrayBuilder &with(StaticSizeBuffer<T> *buf, BufferTarget target);

    VertexArrayBuilder &vec(const std::string &name, GLint size, bool normalized = false);

    std::unique_ptr<VertexArray> build();

private:
    GLuint id_;
    GladGLContext &gl_;

    std::optional<BufferTarget> current_target_{std::nullopt};
    GLsizei current_buf_item_size_{0};
    GLenum current_buf_type_;
    Shader *attrib_lookup_{nullptr};

    struct AttribInfo {
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei offset;
    };
    std::vector<AttribInfo> attrib_info_buf_{};

    void flush_();

    template<typename T>
    static constexpr GLenum determine_buf_type_();
};

template<typename T>
    requires mizu::IsAnyOf<T, float, int, unsigned int>
VertexArrayBuilder &VertexArrayBuilder::with(StaticSizeBuffer<T> *buf, BufferTarget target) {
    flush_();
    buf->bind(target);

    current_target_ = target;
    current_buf_item_size_ = sizeof(T);
    current_buf_type_ = determine_buf_type_<T>();

    return *this;
}

template<typename T>
constexpr GLenum VertexArrayBuilder::determine_buf_type_() {
    if constexpr (std::is_same_v<T, float>)
        return GL_FLOAT;
    else if constexpr (std::is_same_v<T, int>)
        return GL_INT;
    else if constexpr (std::is_same_v<T, unsigned int>)
        return GL_UNSIGNED_INT;
    else
        std::unreachable();
}
} // namespace gloo

#endif // GLOO_VERTEX_ARRAY_HPP
