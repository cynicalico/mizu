#include "mizu/util/memusage.hpp"
#include "mizu/util/platform.hpp"

#if defined(MIZU_PLATFORM_WINDOWS)
// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on
#endif

namespace mizu {
#if defined(MIZU_PLATFORM_WINDOWS)
double memusage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&pmc), sizeof(pmc));
    return pmc.PrivateUsage / 1'048'576.0;
}
#else
double memusage() {
    return 0;
}
#endif
} // namespace mizu
