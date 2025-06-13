#ifndef GLOO_BUFFER_HPP
#define GLOO_BUFFER_HPP

#include <glad/gl.h>
#include "mizu/util/class_helpers.hpp"
#include "mizu/core/log.hpp"
#include "mizu/util/enum_class_helpers.hpp"

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

enum class FillMode { FrontToBack, BackToFront };

template<typename T>
class StaticSizeBuffer : public Buffer {
public:
    StaticSizeBuffer(GladGLContext &gl, std::size_t capacity, FillMode fill_mode = FillMode::FrontToBack);
    ~StaticSizeBuffer();

    NO_COPY(StaticSizeBuffer)

    MOVE_CONSTRUCTOR(StaticSizeBuffer);
    MOVE_ASSIGN_OP(StaticSizeBuffer);

    std::size_t front() const;
    std::size_t size() const;
    std::size_t is_full() const;

    void push(std::initializer_list<T> vs);

    void clear();

    void sync_gl(BufferTarget target);

private:
    FillMode fill_mode_;

    std::size_t gl_buf_capacity_;
    std::size_t gl_buf_pos_;

    T *data_;
    std::size_t data_capacity_;
    std::size_t data_pos_;
};

template<typename T>
StaticSizeBuffer<T>::StaticSizeBuffer(GladGLContext &gl, std::size_t capacity, FillMode fill_mode)
    : Buffer(gl),
      fill_mode_(fill_mode),
      gl_buf_capacity_(0),
      gl_buf_pos_(0),
      data_(new T[capacity]),
      data_capacity_(capacity),
      data_pos_(fill_mode == FillMode::FrontToBack ? 0 : data_capacity_) {}

template<typename T>
StaticSizeBuffer<T>::~StaticSizeBuffer() {
    delete[] data_;
}

template<typename T>
MOVE_CONSTRUCTOR_IMPL_TEMPLATE(StaticSizeBuffer, T)
    : Buffer(std::move(other)),
      fill_mode_(other.fill_mode_),
      gl_buf_capacity_(other.gl_buf_capacity_),
      gl_buf_pos_(other.gl_buf_pos_),
      data_(other.data_),
      data_capacity_(other.data_capacity_),
      data_pos_(other.data_pos_) {
    other.fill_mode_ = FillMode::FrontToBack;
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

        fill_mode_ = other.fill_mode_;
        other.fill_mode_ = FillMode::FrontToBack;

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
std::size_t StaticSizeBuffer<T>::front() const {
    if (fill_mode_ == FillMode::FrontToBack)
        return 0;
    return data_pos_;
}

template<typename T>
std::size_t StaticSizeBuffer<T>::size() const {
    if (fill_mode_ == FillMode::FrontToBack)
        return data_pos_;
    return data_capacity_ - data_pos_;
}

template<typename T>
std::size_t StaticSizeBuffer<T>::is_full() const {
    if (fill_mode_ == FillMode::FrontToBack)
        return data_pos_ == data_capacity_;
    return data_pos_ == 0;
}

template<typename T>
void StaticSizeBuffer<T>::push(std::initializer_list<T> vs) {
    if (fill_mode_ == FillMode::FrontToBack) {
        assert(data_pos_ + vs.size() <= data_capacity_);
        for (const auto v: vs)
            data_[data_pos_++] = v;
    } else {
        assert(data_pos_ >= vs.size());
        for (const auto &[i, v]: std::views::enumerate(vs))
            data_[data_pos_ - vs.size() + i] = v;
        data_pos_ -= vs.size();
    }
}

template<typename T>
void StaticSizeBuffer<T>::clear() {
    if (fill_mode_ == FillMode::FrontToBack) {
        gl_buf_pos_ = 0;
        data_pos_ = 0;
    } else {
        gl_buf_pos_ = data_capacity_;
        data_pos_ = data_capacity_;
    }
}

template<typename T>
void StaticSizeBuffer<T>::sync_gl(BufferTarget target) {
    if (gl_buf_capacity_ != 0 && gl_buf_pos_ == data_pos_)
        return;
    bind(target);

    if (gl_buf_capacity_ == 0) {
        gl_.BufferData(unwrap(target), data_capacity_ * sizeof(T), data_, GL_STREAM_DRAW);
        CHECK_GL_ERROR(gl_, BufferData);
        gl_buf_capacity_ = data_capacity_;
        MIZU_LOG_TRACE("Initialized GL buffer id={}", id);
    } else if (fill_mode_ == FillMode::FrontToBack) {
        assert(gl_buf_pos_ < data_pos_);
        gl_.BufferSubData(
                unwrap(target), gl_buf_pos_ * sizeof(T), (data_pos_ - gl_buf_pos_) * sizeof(T), data_ + gl_buf_pos_
        );
        CHECK_GL_ERROR(gl_, BufferSubData);
    } else {
        assert(gl_buf_pos_ > data_pos_);
        gl_.BufferSubData(
                unwrap(target), data_pos_ * sizeof(T), (gl_buf_pos_ - data_pos_) * sizeof(T), data_ + data_pos_
        );
        CHECK_GL_ERROR(gl_, BufferSubData);
    }
    gl_buf_pos_ = data_pos_;

    unbind(target);
}
} // namespace gloo

#endif // GLOO_BUFFER_HPP
