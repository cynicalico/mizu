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

#endif // MIZU_LOG_HPP
