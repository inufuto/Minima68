#pragma once

#include <cstdint>
#include "Sound.h"
#include "../../core/Uncopyable.h"

// class SoundChannel : public 
// {
// public:
    
// private:
// public:
//     auto SetVolume() const { return volume; }
//     virtual uint8_t Sample() = 0;
// };

class ToneChannel : public Uncopyable
{
private:
	const uint8_t* pSamples = nullptr;
    uint8_t volume = MaxVolume;
    uint32_t phaseDelta = 0;
    int32_t phaseError = 0;
    uint8_t sampleIndex = 0;
public:
    void SetSamples(const uint8_t* pSamples) { this->pSamples = pSamples; }
    void SetFrequency(uint16_t frequency);
    void SetVolume(uint8_t volume) { this->volume = volume; }
    uint8_t Sample();
};

extern ToneChannel toneChannels[ToneChannelCount];

extern void InitSound();