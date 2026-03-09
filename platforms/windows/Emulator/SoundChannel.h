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


class SoundChannel : public Uncopyable
{
protected:
	static float ToFloat(uint8_t b);
public:
	//void SourceSamples(const uint8_t* pSamples) { this->pSourceSamples = pSamples; }
	//auto SourceSampleAt(int index) const { return pSourceSamples[index]; }
	////void SourceChanged() { sourceChanged = true; }
	//float Volume() const { return volume; }
};

class ToneChannel : public SoundChannel
{
private:
	const uint8_t* pSourceSamples;
	float samples[ToneSampleCount];
	uint16_t frequency;
	float volume = 1.0f;
	double phase = 0;
	double step;
	bool sourceChanged = true;
protected:
	void UpdateSamples();
public:
	void SourceSamples(const uint8_t* pSamples) { this->pSourceSamples = pSamples; }
	void Frequency(uint16_t frequency);
	float Sample();
};

class EffectChannel : public SoundChannel
{
private:
	const uint8_t* pSourceSamples;
	float samples[EffectSampleCount];
};

extern void StartSound();