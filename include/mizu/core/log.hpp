#ifndef MIZU_LOG_HPP
#define MIZU_LOG_HPP

#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif

#include "fmt/chrono.h" // Allow logging chrono types
#include "fmt/ranges.h" // Allow logging ranges (vector, etc.)
#include "fmt/std.h" // Allow logging STL types
#include "fmt/xchar.h" // Allow logging wchar

#include "spdlog/spdlog.h"

#define MIZU_LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define MIZU_LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define MIZU_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define MIZU_LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define MIZU_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define MIZU_LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)

#ifndef NDEBUG
#include <glad/gl.h>

namespace mizu {
inline void check_gl_error(const GladGLContext &ctx, const char *fn_name) {
    auto err = ctx.GetError();
    switch (err) {
    case GL_NO_ERROR: return;
    case GL_INVALID_ENUM: MIZU_LOG_ERROR("glGetError {}: GL_INVALID_ENUM", fn_name); break;
    case GL_INVALID_VALUE: MIZU_LOG_ERROR("glGetError {}: GL_INVALID_VALUE", fn_name); break;
    case GL_INVALID_OPERATION: MIZU_LOG_ERROR("glGetError {}: GL_INVALID_OPERATION", fn_name); break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        MIZU_LOG_ERROR("glGetError {}: GL_INVALID_FRAMEBUFFER_OPERATION", fn_name);
        break;
    case GL_OUT_OF_MEMORY: MIZU_LOG_ERROR("glGetError {}: GL_OUT_OF_MEMORY", fn_name); break;
    case GL_STACK_UNDERFLOW: MIZU_LOG_ERROR("glGetError {}: GL_STACK_UNDERFLOW", fn_name); break;
    case GL_STACK_OVERFLOW: MIZU_LOG_ERROR("glGetError {}: GL_STACK_OVERFLOW", fn_name); break;
    default: std::unreachable();
    }
}
} // namespace mizu

#define CHECK_GL_ERROR(ctx, fn_name) mizu::check_gl_error((ctx), #fn_name)
#else
#define CHECK_GL_ERROR(ctx, fn_name) do { } while(0)
#endif

#endif // MIZU_LOG_HPP
