#pragma once

#include <cstdint>
#include "Sound.h"
#include "../../core/Uncopyable.h"

class SoundChannel : public Uncopyable
{
public:
    static constexpr auto PwmWrap = 0x100;
private:
	const uint8_t* pSamples;
    uint8_t volume = MaxVolume;
public:
    void Samples(const uint8_t* pSamples) { this->pSamples = pSamples; }
    auto SampleAt(int index) const { return pSamples[index]; }
    auto Volume() const { return volume; }
    virtual uint8_t Sample() = 0;
};

class ToneChannel : public SoundChannel
{
private:
	double step;
	double phase = 0;
public:
    void SetFrequency(uint16_t frequency);
    uint8_t Sample() override;
};

extern void InitSound();