#include "../core/Minima68.h"
#include "MasterClock.h"

class Minima68Pico : public Minima68
{
private:
    MasterClock masterClock;
public:
    Minima68Pico() : Minima68(), masterClock(MasterClockFrequency) {}
    void Run();
};