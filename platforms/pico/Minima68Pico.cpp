#include <cstring>
#include "Minima68.h"
#include "Minima68Pico.h"

extern const uint8_t testCode[];

void Minima68Pico::Run()
{
    masterClock.AddDestination(&Cpu());
    memcpy(Minima68::Ram() + 0x100, testCode, 0x100);
    Reset();
    masterClock.Run();
}