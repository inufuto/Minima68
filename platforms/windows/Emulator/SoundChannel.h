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
	uint32_t phase = 0;
	uint8_t volume = 0;
	uint32_t phaseDelta = 0;
public:
	void SetVolume(uint8_t volume);
	void SetFrequency(uint16_t frequency);
	float Sample();
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
	float Sample();
};

extern ToneChannel ToneChannels[ToneChannelCount];
extern EffectChannel EffectChannel;