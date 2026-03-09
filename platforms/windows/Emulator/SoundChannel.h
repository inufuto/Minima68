#pragma once

#include "../Audio/WindowsAudioClient.h"
#include "../../../core/Sound.h"

class Minima68Win;

class SoundThread : public Uncopyable
{
private:
	WindowsAudioClient audioClient;
	HANDLE hThread = nullptr;
	bool running;
private:
	static unsigned __stdcall ThreadProc(void* pThis);
	void Loop() const;
public:
	~SoundThread() override { Stop(); }
	auto SampleRate() const { return audioClient.SampleRate(); }
	virtual void Start(Minima68Win* pEmulator);
	void Stop();
};
extern SoundThread SoundThread;


class ToneChannel : public Uncopyable
{
private:
	const uint8_t* pSourceSamples = nullptr;
	float samples[ToneSampleCount];
	uint16_t frequency;
	float volume = 0.0f;
	double phase = 0;
	double step;
	bool sourceChanged = true;
protected:
	void UpdateSamples();
public:
	void SetSourceSamples(const uint8_t* pSamples);
	void SetFrequency(uint16_t frequency);
	void SetVolume(float volume) { this->volume = volume; }
	float Sample();
};

class EffectChannel : public Uncopyable
{
private:
	const uint8_t* pSourceSamples;
	float samples[EffectSampleCount];
};

extern ToneChannel ToneChannels[ToneChannelCount];
