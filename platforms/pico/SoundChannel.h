#pragma once

#include <cstdint>
#include "Sound.h"
#include "../../core/Uncopyable.h"

class ToneChannel : public Uncopyable
{
private:
	const uint8_t* pSamples = nullptr;
    uint8_t volume = 0;
    uint32_t phaseDelta = 0;
    int32_t phaseError = 0;
    uint8_t sampleIndex = 0;
public:
    void SetSamples(const uint8_t* pSamples) { this->pSamples = pSamples; }
    void SetFrequency(uint16_t frequency);
    void SetVolume(uint8_t volume);
    uint8_t Sample();
};

class EffectChannel : public Uncopyable
{
private:
    const uint8_t* pSamples = nullptr;
    uint8_t volume = 0;
    uint8_t rate = 0;
    uint32_t phase = 0;
    uint32_t sampleIndex = 0;
public:
    void SetSamples(const uint8_t* pSamples) { this->pSamples = pSamples; }
    void SetVolume(uint8_t volume);
    void SetRate(uint8_t rate);
    uint8_t Sample();
};

extern ToneChannel toneChannels[ToneChannelCount];
extern EffectChannel effectChannel;

extern void InitSound();