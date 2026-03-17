#pragma once

#include <cstdint>
#include "Sound.h"
#include "../../core/Uncopyable.h"

class ToneChannel : public Uncopyable
{
private:
    uint32_t phase = 0;
    uint8_t volume = 0;
    uint32_t phaseDelta = 0;
public:
    void SetFrequency(uint16_t frequency);
    void SetVolume(uint8_t volume);
    uint8_t Sample();
};

class EffectChannel : public Uncopyable
{
private:
    uint32_t noiseState = 1;
    uint8_t volume = 0;
    uint32_t phase = 0;
    uint32_t phaseDelta = 0;
public:
    void SetVolume(uint8_t volume);
    void SetFrequency(uint16_t frequency);
    void SetRate(uint8_t rate);
    uint8_t Sample();
};

extern ToneChannel toneChannels[ToneChannelCount];
extern EffectChannel effectChannel;

extern void InitSound();