#include "mizu/memusage.hpp"
#include "mizu/platform.hpp"

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
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

    return virtualMemUsedByMe / 1'048'576.0;
}
#endif
} // namespace mizu
