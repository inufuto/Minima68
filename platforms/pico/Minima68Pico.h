#include "../core/Minima68.h"
#include "PrimaryClock.h"

extern class Minima68Pico : public Minima68
{
private:
    PrimaryClock primaryClock;
public:
    Minima68Pico() : Minima68(), primaryClock(PrimaryClockFrequency) {}
    void Run();
    void SetColor(int index, uint8_t r, uint8_t g, uint8_t b) override;
} emulator;