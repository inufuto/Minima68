#include "pico/stdlib.h"
#include "PrimaryClock.h"

void PrimaryClock::Run()
{
    time = 0;
    auto last = time_us_64();
    while (true) {
        auto now = time_us_64();
        auto elapsed = static_cast<double>(now - last) / 1'000'000.0;
        last = now;
        auto cyclesToRun = static_cast<uint32_t>(targetFrequency * elapsed);
        for (uint32_t i = 0; i < cyclesToRun; i++) {
            OnClock(time);
            ++time;
        }
    }
}
