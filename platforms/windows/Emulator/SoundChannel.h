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
	bool sourceChanged = true;
	float samples[ToneSampleCount];
	uint8_t volume = 0;
	double phase = 0;
	double step;
private:
	void UpdateSamples();
public:
	void SetSourceSamples(const uint8_t* pSamples);
	void SetVolume(uint8_t volume);
	void SetFrequency(uint16_t frequency);
	float Sample();
};

class EffectChannel : public Uncopyable
{
private:
	const uint8_t* pSourceSamples = nullptr;
	bool sourceChanged = true;
	float samples[EffectSampleCount];
	uint8_t volume = 0;
	double phase = 0;
	double step;
private:
	void UpdateSamples();
public:
	void SetSourceSamples(const uint8_t* pSamples);
	void SetVolume(uint8_t volume);
	void SetRate(uint8_t rate);
	float Sample();
};

extern ToneChannel ToneChannels[ToneChannelCount];
extern EffectChannel EffectChannel;