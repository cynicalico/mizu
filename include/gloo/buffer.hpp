#ifndef GLOO_BUFFER_HPP
#define GLOO_BUFFER_HPP

#include <glad/gl.h>
#include "mizu/class_helpers.hpp"
#include "mizu/enum_class_helpers.hpp"
#include "mizu/log.hpp"

namespace gloo {
enum class BufferTarget : GLenum {
    Array = GL_ARRAY_BUFFER,
    AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
    CopyRead = GL_COPY_READ_BUFFER,
    CopyWrite = GL_COPY_WRITE_BUFFER,
    DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
    DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    PixelPack = GL_PIXEL_PACK_BUFFER,
    PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
    Query = GL_QUERY_BUFFER,
    ShaderStorage = GL_SHADER_STORAGE_BUFFER,
    Texture = GL_TEXTURE_BUFFER,
    TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
    Uniform = GL_UNIFORM_BUFFER,
};

template<typename T>
class Buffer {
public:
    GLuint id{0};

    Buffer(GladGLContext &gl, std::size_t capacity);
    ~Buffer();

    NO_COPY(Buffer)

    MOVE_CONSTRUCTOR(Buffer);
    MOVE_ASSIGN_OP(Buffer);

    void push(std::initializer_list<T> vs);

    void sync_gl(BufferTarget target);
    void bind(BufferTarget target);
    void unbind(BufferTarget target);

private:
    GladGLContext &gl_;
    std::size_t gl_buf_capacity_;
    std::size_t gl_buf_pos_;

    T *data_;
    std::size_t data_capacity_;
    std::size_t data_pos_;
};

template<typename T>
Buffer<T>::Buffer(GladGLContext &gl, std::size_t capacity)
    : gl_(gl), gl_buf_capacity_(0), gl_buf_pos_(0), data_(), data_capacity_(capacity), data_pos_(0) {
    data_ = new T[capacity];
    gl_.GenBuffers(1, &id);
    SPDLOG_TRACE("Created buffer id={}", id);
}

template<typename T>
Buffer<T>::~Buffer() {
    if (id != 0) {
        gl_.DeleteBuffers(1, &id);
        SPDLOG_TRACE("Deleted buffer id={}", id);
    }
    // delete[] data_;
}

template<typename T>
MOVE_CONSTRUCTOR_IMPL_TEMPLATE(Buffer, T)
    : id(other.id), gl_(other.gl_), gl_buf_capacity_(other.gl_buf_capacity_), gl_buf_pos_(other.gl_buf_pos_),
      data_(other.data_), data_capacity_(other.data_capacity_), data_pos_(other.data_pos_) {
    other.id = 0;
    other.gl_buf_capacity_ = 0;
    other.gl_buf_pos_ = 0;
    other.data_ = nullptr;
    other.data_capacity_ = 0;
    other.data_pos_ = 0;
}

template<typename T>
MOVE_ASSIGN_OP_IMPL_TEMPLATE(Buffer, T) {
    if (this != &other) {
        id = other.id;
        other.id = 0;

        gl_ = other.gl_;

        gl_buf_capacity_ = other.gl_buf_capacity_;
        other.gl_buf_capacity_ = 0;

        gl_buf_pos_ = other.gl_buf_pos_;
        other.gl_buf_pos_ = 0;

        data_ = other.data_;
        other.data_ = nullptr;

        data_capacity_ = other.data_capacity_;
        other.data_capacity_ = 0;

        data_pos_ = other.data_pos_;
        other.data_pos_ = 0;
    }
    return *this;
}

template<typename T>
void Buffer<T>::push(std::initializer_list<T> vs) {
    assert(data_pos_ + vs.size() <= data_capacity_);
    for (const auto v: vs)
        data_[data_pos_++] = v;
}

template<typename T>
void Buffer<T>::sync_gl(BufferTarget target) {
    if (gl_buf_pos_ == data_pos_) return;
    bind(target);

    if (gl_buf_capacity_ == 0) {
        gl_.BufferData(unwrap(target), data_capacity_ * sizeof(T), data_, GL_STATIC_DRAW);
        gl_buf_capacity_ = data_capacity_;
        SPDLOG_TRACE("Initialized GL buffer id={}", id);
    } else {
        gl_.BufferSubData(unwrap(target), gl_buf_pos_ * sizeof(T), (data_pos_ - gl_buf_pos_) * sizeof(T), data_);
    }
    gl_buf_pos_ = data_pos_;

    unbind(target);
}

template<typename T>
void Buffer<T>::bind(BufferTarget target) {
    gl_.BindBuffer(unwrap(target), id);
}

template<typename T>
void Buffer<T>::unbind(BufferTarget target) {
    gl_.BindBuffer(unwrap(target), 0);
}
} // namespace gloo

#endif // GLOO_BUFFER_HPP
