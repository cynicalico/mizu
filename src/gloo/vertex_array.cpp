#include "gloo/vertex_array.hpp"

namespace gloo {
VertexArray::~VertexArray() {
    if (id != 0) {
        gl_.DeleteVertexArrays(1, &id);
        CHECK_GL_ERROR(gl_, DeleteVertexArrays);
        MIZU_LOG_TRACE("Deleted vertex array id={}", id);
    }
}

MOVE_CONSTRUCTOR_IMPL(VertexArray)
    : id(other.id), gl_(other.gl_) {
    other.id = 0;
}

MOVE_ASSIGN_OP_IMPL(VertexArray) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        gl_ = other.gl_;
    }
    return *this;
}

void VertexArray::bind() {
    gl_.BindVertexArray(id);
    CHECK_GL_ERROR(gl_, BindVertexArray);
}

void VertexArray::unbind() {
    gl_.BindVertexArray(0);
    CHECK_GL_ERROR(gl_, BindVertexArray);
}

void VertexArray::draw_arrays(DrawMode mode, std::size_t first, std::size_t count) {
    bind();
    gl_.DrawArrays(unwrap(mode), first, count);
    CHECK_GL_ERROR(gl_, DrawArrays);
    unbind();
}

VertexArray::VertexArray(GladGLContext &gl, GLuint id)
    : id(id), gl_(gl) {}

VertexArrayBuilder::VertexArrayBuilder(GladGLContext &gl)
    : gl_(gl) {
    gl_.GenVertexArrays(1, &id_);
    CHECK_GL_ERROR(gl_, GenVertexArrays);
    MIZU_LOG_TRACE("Created vertex array id={}", id_);

    gl_.BindVertexArray(id_);
    CHECK_GL_ERROR(gl_, BindVertexArray);
}

VertexArrayBuilder &VertexArrayBuilder::with(Shader *shader) {
    attrib_lookup_ = shader;

    return *this;
}

VertexArrayBuilder &VertexArrayBuilder::vec(const std::string &name, GLint size, bool normalized) {
    GLsizei offset = 0;
    for (const auto &attrib_info: attrib_info_buf_)
        offset += attrib_info.size;

    if (auto loc = attrib_lookup_->attrib_location(name); loc)
        attrib_info_buf_.emplace_back(*loc, size, current_buf_type_, normalized ? GL_TRUE : GL_FALSE, offset);

    return *this;
}

std::unique_ptr<VertexArray> VertexArrayBuilder::build() {
    flush_();

    gl_.BindVertexArray(0);
    CHECK_GL_ERROR(gl_, BindVertexArray);
    return std::unique_ptr<VertexArray>(new VertexArray(gl_, id_));
}

void VertexArrayBuilder::flush_() {
    if (attrib_info_buf_.empty())
        return;

    GLsizei stride = 0;
    for (const auto &attrib_info: attrib_info_buf_)
        stride += attrib_info.size;

    for (const auto &attrib_info: attrib_info_buf_) {
        MIZU_LOG_TRACE(
                "VertexAttribPointer: index={} size={} type={} normalized={} stride={} offset={}",
                attrib_info.index,
                attrib_info.size,
                attrib_info.type,
                attrib_info.normalized,
                stride * current_buf_item_size_,
                attrib_info.offset * current_buf_item_size_
        );

        gl_.VertexAttribPointer(
                attrib_info.index,
                attrib_info.size,
                attrib_info.type,
                attrib_info.normalized,
                stride * current_buf_item_size_,
                reinterpret_cast<void *>(static_cast<uintptr_t>(attrib_info.offset * current_buf_item_size_))
        );
        CHECK_GL_ERROR(gl_, VertexAttribPointer);
        gl_.EnableVertexAttribArray(attrib_info.index);
        CHECK_GL_ERROR(gl_, EnableVertexAttribArray);
    }

    attrib_info_buf_.clear();
    if (current_target_) {
        gl_.BindBuffer(unwrap(*current_target_), 0);
        CHECK_GL_ERROR(gl_, BindBuffer);
    }
}
} // namespace gloo
