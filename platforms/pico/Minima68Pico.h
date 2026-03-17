#pragma once

#include "../../core/Minima68.h"
#include "PrimaryClock.h"
#include "MemoryMap.h"

extern class Minima68Pico : public Minima68
{
private:
    PrimaryClock primaryClock;
protected:
    void SetToneFrequency(int index, uint16_t frequency) override;
    void SetToneVolume(int index, uint8_t volume) override;

    void SetEffectFrequency(uint16_t frequency) override;
    void SetEffectVolume(uint8_t volume) override;
public:
    Minima68Pico() : Minima68(), primaryClock(PrimaryClockFrequency) {}
    void Run();
    uint8_t ReadMemory(uint16_t address) const override;
    void SetJoystickBits(uint8_t bits) { Ram()[JoystickAddress] = bits; }
    void SetColor(int index, uint8_t r, uint8_t g, uint8_t b) override;
    void SetPage(uint8_t page) override;
} emulator;