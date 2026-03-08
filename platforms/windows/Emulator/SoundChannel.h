#pragma once

#include "../Audio/WindowsAudioClient.h"

class SoundChannel : public Uncopyable
{
private:
	WindowsAudioClient audioClient;
	HANDLE hThread;
	bool running;
	float volume = 0.5f;
private:
	static unsigned __stdcall ThreadProc(void* pThis);
protected:
	static float ToFloat(uint8_t b);
	auto SampleRate() const { return audioClient.SampleRate(); }
public:
	~SoundChannel() override { Stop(); }
	float Volume() const { return volume; }
	virtual void Start();
	void Stop();
	virtual void Write(float* pBuffer, UINT32 framesToWrite) = 0;
	void Loop();
};

class ToneChannel : public SoundChannel
{
private:
	static constexpr auto SampleCount = 32;
	const uint8_t* pSourceSamples;
	float samples[SampleCount];
	uint16_t frequency = 440;
	double step;
	double phase;
private:
	void UpdateSamples();
protected:
	void Write(float* pBuffer, UINT32 framesToWrite) override;
public:
	explicit ToneChannel(const uint8_t* pSourceSamples) : pSourceSamples(pSourceSamples)
	{
		UpdateSamples();
	}
	void Start() override;
};

class EffectChannel : public SoundChannel
{
private:
	static constexpr auto SampleCount = 128;
	const uint8_t* pSourceSamples;
	float samples[SampleCount];
};