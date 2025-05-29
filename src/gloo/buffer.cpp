#include "gloo/buffer.hpp"

namespace gloo {
Buffer::Buffer(GladGLContext &gl) : gl_(gl) {
    gl_.GenBuffers(1, &id);
    SPDLOG_TRACE("Created buffer id={}", id);
}

Buffer::~Buffer() {
    if (id != 0) {
        gl_.DeleteBuffers(1, &id);
        SPDLOG_TRACE("Deleted buffer id={}", id);
    }
}

Buffer::MOVE_CONSTRUCTOR_IMPL(Buffer) : id(other.id), gl_(other.gl_) {
    other.id = 0;
}

MOVE_ASSIGN_OP_IMPL(Buffer) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        gl_ = other.gl_;
    }
    return *this;
}

void Buffer::bind(BufferTarget target) {
    gl_.BindBuffer(unwrap(target), id);
}

void Buffer::unbind(BufferTarget target) {
    gl_.BindBuffer(unwrap(target), 0);
}
} // namespace gloo
