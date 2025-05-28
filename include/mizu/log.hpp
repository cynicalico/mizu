#ifndef MIZU_LOG_HPP
#define MIZU_LOG_HPP

#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include "spdlog/spdlog.h"

#endif // MIZU_LOG_HPP
