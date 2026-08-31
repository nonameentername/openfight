#include <cstdint>

extern "C" int __isPlatformVersionAtLeast(uint32_t platform, uint32_t major, uint32_t minor, uint32_t patch) {
    (void)platform;
    (void)major;
    (void)minor;
    (void)patch;
    return 0;
}
