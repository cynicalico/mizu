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

class Buffer {
public:
    GLuint id{0};

    Buffer(GladGLContext &gl);
    ~Buffer();

    NO_COPY(Buffer)

    MOVE_CONSTRUCTOR(Buffer);
    MOVE_ASSIGN_OP(Buffer);

    void bind(BufferTarget target);
    void unbind(BufferTarget target);

protected:
    GladGLContext &gl_;
};

template<typename T>
class StaticSizeBuffer : public Buffer {
public:
    StaticSizeBuffer(GladGLContext &gl, std::size_t capacity);
    ~StaticSizeBuffer();

    NO_COPY(StaticSizeBuffer)

    MOVE_CONSTRUCTOR(StaticSizeBuffer);
    MOVE_ASSIGN_OP(StaticSizeBuffer);

    std::size_t front() const { return 0; }
    std::size_t size() const { return data_pos_; }
    std::size_t is_full() const { return data_pos_ == data_capacity_; }

    void push(std::initializer_list<T> vs);

    void clear();

    void sync_gl(BufferTarget target);

private:
    std::size_t gl_buf_capacity_;
    std::size_t gl_buf_pos_;

    T *data_;
    std::size_t data_capacity_;
    std::size_t data_pos_;
};

template<typename T>
StaticSizeBuffer<T>::StaticSizeBuffer(GladGLContext &gl, std::size_t capacity)
    : Buffer(gl), gl_buf_capacity_(0), gl_buf_pos_(0), data_(), data_capacity_(capacity), data_pos_(0) {
    data_ = new T[capacity];
}

template<typename T>
StaticSizeBuffer<T>::~StaticSizeBuffer() {
    delete[] data_;
}

template<typename T>
MOVE_CONSTRUCTOR_IMPL_TEMPLATE(StaticSizeBuffer, T)
    : Buffer(std::move(other)),
      gl_buf_capacity_(other.gl_buf_capacity_),
      gl_buf_pos_(other.gl_buf_pos_),
      data_(other.data_),
      data_capacity_(other.data_capacity_),
      data_pos_(other.data_pos_) {
    other.gl_buf_capacity_ = 0;
    other.gl_buf_pos_ = 0;
    other.data_ = nullptr;
    other.data_capacity_ = 0;
    other.data_pos_ = 0;
}

template<typename T>
MOVE_ASSIGN_OP_IMPL_TEMPLATE(StaticSizeBuffer, T) {
    if (this != &other) {
        Buffer::operator=(other);

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
void StaticSizeBuffer<T>::push(std::initializer_list<T> vs) {
    assert(data_pos_ + vs.size() <= data_capacity_);
    for (const auto v: vs)
        data_[data_pos_++] = v;
}

template<typename T>
void StaticSizeBuffer<T>::clear() {
    gl_buf_pos_ = 0;
    data_pos_ = 0;
}

template<typename T>
void StaticSizeBuffer<T>::sync_gl(BufferTarget target) {
    if (gl_buf_pos_ == data_pos_)
        return;
    bind(target);

    if (gl_buf_capacity_ == 0) {
        gl_.BufferData(unwrap(target), data_capacity_ * sizeof(T), data_, GL_STATIC_DRAW);
        gl_buf_capacity_ = data_capacity_;
        SPDLOG_TRACE("Initialized GL buffer id={}", id);
    } else {
        gl_.BufferSubData(
                unwrap(target), gl_buf_pos_ * sizeof(T), (data_pos_ - gl_buf_pos_) * sizeof(T), data_ + gl_buf_pos_
        );
    }
    gl_buf_pos_ = data_pos_;

    unbind(target);
}
} // namespace gloo

#endif // GLOO_BUFFER_HPP
