#ifndef MIZU_LOG_HPP
#define MIZU_LOG_HPP

#if !defined(SPDLOG_ACTIVE_LEVEL)
#if !defined(NDEBUG)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif
#endif
#include "spdlog/spdlog.h"

#endif // MIZU_LOG_HPP
