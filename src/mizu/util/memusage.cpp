#include "mizu/util/memusage.hpp"
#include "mizu/util/platform.hpp"

#if defined(MIZU_PLATFORM_WINDOWS)
// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on
#else
#include <cstdio>
#include <sys/resource.h>
#include <unistd.h>
#endif

namespace mizu {
double memusage() {
#if defined(MIZU_PLATFORM_WINDOWS)
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&pmc), sizeof(pmc));
    return pmc.PrivateUsage / 1'048'576.0;
#else
    long rss = 0L;
    FILE *fp;
    if ((fp = fopen("/proc/self/statm", "r")) == nullptr)
        return 0.0; // Can't open?
    if (fscanf(fp, "%*s%ld", &rss) != 1) {
        fclose(fp);
        return 0.0; // Can't read?
    }
    fclose(fp);
    return static_cast<double>(static_cast<size_t>(rss) * static_cast<size_t>(sysconf(_SC_PAGESIZE))) / 1'048'576.0;
#endif
}
} // namespace mizu
