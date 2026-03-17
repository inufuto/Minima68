#include <cstring>

#include "Minima68Pico.h"
#include "Ntsc.h"
#include "MemoryMap.h"
#include "Input.h"
#include "SoundChannel.h"

Minima68Pico emulator;

void Minima68Pico::Run()
{
    primaryClock.AddDestination(&Cpu());
    Reset();
    primaryClock.Run();
}

uint8_t Minima68Pico::ReadMemory(uint16_t address) const
{
    auto value = Minima68::ReadMemory(address);
    if (address == JoystickAddress) {
        value |= ReadButtons();
    }
    return value;
}

void Minima68Pico::SetColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
    ::SetColor(index, r, g, b);
}

void Minima68Pico::SetPage(uint8_t page)
{
    ::SetPage(page);
}

void Minima68Pico::SetToneFrequency(int index, uint16_t frequency)
{
    toneChannels[index].SetFrequency(frequency);
}

void Minima68Pico::SetToneVolume(int index, uint8_t volume)
{
    toneChannels[index].SetVolume(volume);
}

void Minima68Pico::SetEffectFrequency(uint16_t frequency)
{
    effectChannel.SetFrequency(frequency);
}

void Minima68Pico::SetEffectVolume(uint8_t volume)
{
    effectChannel.SetVolume(volume);
}
