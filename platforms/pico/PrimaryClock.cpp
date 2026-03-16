#include <pico/stdlib.h>
#include <tusb.h>
#include <hid.h>

#include "PrimaryClock.h"

void __not_in_flash_func(PrimaryClock::Run)()
{
    time = 0;
    const uint64_t freq = static_cast<uint64_t>(targetFrequency + 0.5); // cycles per second (integer)
    uint64_t accumulator = 0;
    uint64_t last = time_us_64();
    while (true) {
        uint64_t now = time_us_64();
        accumulator += (now - last) * freq;
        last = now;
        uint32_t cyclesToRun = static_cast<uint32_t>(accumulator / 1'000'000ULL);
        accumulator %= 1'000'000ULL;
        for (uint32_t i = 0; i < cyclesToRun; i++) {
            OnClock(time);
            ++time;
            if ((i & 0x3fff) == 0) {
                tuh_task();
            }
        }
    }
}
