#include "gloo/vertex_array.hpp"

namespace gloo {
VertexArray::~VertexArray() {
    if (id != 0) {
        gl_.DeleteVertexArrays(1, &id);
        SPDLOG_TRACE("Deleted vertex array id={}", id);
    }
}

MOVE_CONSTRUCTOR_IMPL(VertexArray) : id(other.id), gl_(other.gl_) {
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
}

void VertexArray::unbind() {
    gl_.BindVertexArray(0);
}

void VertexArray::draw_arrays(DrawMode mode, std::size_t first, std::size_t count) {
    bind();
    gl_.DrawArrays(unwrap(mode), first, count);
    unbind();
}

VertexArray::VertexArray(GladGLContext &gl, GLuint id) : id(id), gl_(gl) {}

VertexArrayBuilder::VertexArrayBuilder(GladGLContext &gl) : gl_(gl) {
    gl_.GenVertexArrays(1, &id_);
    SPDLOG_TRACE("Created vertex array id={}", id_);

    gl_.BindVertexArray(id_);
}

VertexArrayBuilder &VertexArrayBuilder::from(Shader *shader) {
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
    return std::unique_ptr<VertexArray>(new VertexArray(gl_, id_));
}

void VertexArrayBuilder::flush_() {
    if (attrib_info_buf_.empty()) return;

    GLsizei stride = 0;
    for (const auto &attrib_info: attrib_info_buf_)
        stride += attrib_info.size;

    for (const auto &attrib_info: attrib_info_buf_) {
        SPDLOG_TRACE(
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
        gl_.EnableVertexAttribArray(attrib_info.index);
    }

    attrib_info_buf_.clear();
    if (current_target_) gl_.BindBuffer(unwrap(*current_target_), 0);
}
} // namespace gloo
