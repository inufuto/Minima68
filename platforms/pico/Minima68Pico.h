#pragma once

#include "../core/Minima68.h"
#include "PrimaryClock.h"

extern class Minima68Pico : public Minima68
{
private:
    PrimaryClock primaryClock;
protected:
    void SetColor(int index, uint8_t r, uint8_t g, uint8_t b) override;
    
    void SetToneSample(int index, const uint8_t* pSample) override;
    void SetToneFrequency(int index, uint16_t frequency) override;
    void SetToneVolume(int index, uint8_t volume) override;

    void SetEffectSample(uint8_t* pSamples) override;
    void SetEffectRate(uint8_t rate) override;
    void SetEffectVolume(uint8_t volume) override;
public:
    Minima68Pico() : Minima68(), primaryClock(PrimaryClockFrequency) {}
    void Run();
    uint8_t ReadMemory(uint16_t address) const override;
    void SetJoystickBits(uint8_t bits) { Ram()[JoystickAddress] = bits; }
} emulator;