#include "mizu/platform.hpp"
#include "fmt/ranges.h"
#include "mizu/log.hpp"

namespace mizu {
void log_platform() {
#define STRINGIFY(x) #x
    std::vector<const char *> detected_platforms{};
#if defined(MIZU_PLATFORM_WINDOWS)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_WINDOWS));
#endif
#if defined(MIZU_PLATFORM_WINDOWS32)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_WINDOWS32));
#endif
#if defined(MIZU_PLATFORM_WINDOWS64)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_WINDOWS64));
#endif
#if defined(MIZU_PLATFORM_APPLE_IOS_SIMULATOR)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_APPLE_IOS_SIMULATOR));
#endif
#if defined(MIZU_PLATFORM_APPLE_CATALYST)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_APPLE_CATALYST));
#endif
#if defined(MIZU_PLATFORM_APPLE_IOS)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_APPLE_IOS));
#endif
#if defined(MIZU_PLATFORM_APPLE)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_APPLE));
#endif
#if defined(MIZU_PLATFORM_LINUX)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_LINUX));
#endif
#if defined(MIZU_PLATFORM_UNIX)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_UNIX));
#endif
#if defined(MIZU_PLATFORM_POSIX)
    detected_platforms.push_back(STRINGIFY(MIZU_PLATFORM_POSIX));
#endif
    SPDLOG_DEBUG("Platform(s) detected: {}", fmt::join(detected_platforms, ", "));
#undef STRINGIFY
}
} // namespace mizu
