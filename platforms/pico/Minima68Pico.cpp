#include <cstring>
#include "Minima68.h"
#include "Minima68Pico.h"
#include "Ntsc.h"
#include "MemoryMap.h"

extern const uint8_t testCode[];

Minima68Pico emulator;

const uint8_t PianoWave[] = {
	254,238,204,172,150,128,104,84,
	74,65,50,36,32,33,25,9,
	0,9,25,33,32,36,50,65,
	74,84,104,128,150,172,204,238,
};
const uint8_t Lead2Wave[] = {
	203,160,75,22,17,23,17,10,
	11,10,6,4,4,3,0,0,
	2,0,0,3,4,4,6,10,
	11,10,17,23,17,22,75,160,
};

void Minima68Pico::Run()
{
    primaryClock.AddDestination(&Cpu());
    memcpy(Minima68::Ram() + 0x100, testCode, 0x100);
    Reset();
    memcpy(Minima68::Ram() + ShortWaveAddress, Lead2Wave, 32);
    primaryClock.Run();
}

void Minima68Pico::SetColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
    ::SetColor(index, r, g, b);
}
