#include <cstring>
#include "Minima68.h"
#include "Minima68Pico.h"
#include "Ntsc.h"
#include "MemoryMap.h"

extern const uint8_t testCode[];

Minima68Pico emulator;

void Minima68Pico::Run()
{
    primaryClock.AddDestination(&Cpu());
    memcpy(Minima68::Ram() + 0x100, testCode, 0x100);
    Reset();
    primaryClock.Run();
}

void Minima68Pico::SetColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
    ::SetColor(index, r, g, b);
}
