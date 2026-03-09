#include <cstring>
#include "Minima68.h"
#include "Minima68Pico.h"
#include "Ntsc.h"
#include "MemoryMap.h"
#include "SoundChannel.h"

Minima68Pico emulator;

void Minima68Pico::Run()
{
    primaryClock.AddDestination(&Cpu());
    Reset();
    primaryClock.Run();
}

void Minima68Pico::SetColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
    ::SetColor(index, r, g, b);
}

void Minima68Pico::SetToneSample(int index, const uint8_t *pSample)
{
    toneChannels[index].SetSamples(pSample);
}

void Minima68Pico::SetToneFrequency(int index, uint16_t frequency)
{
    toneChannels[index].SetFrequency(frequency);
}

void Minima68Pico::SetToneVolume(int index, uint8_t volume)
{
    toneChannels[index].SetVolume(volume);
}
