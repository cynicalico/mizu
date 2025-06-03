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

#endif // MIZU_LOG_HPP
