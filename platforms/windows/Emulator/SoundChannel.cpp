#include <process.h>
#include <algorithm>
#include "SoundChannel.h"
#include "Minima68Win.h"
#undef min
#undef max

ToneChannel ToneChannels[ToneChannelCount];
class EffectChannel EffectChannel;
class SoundThread SoundThread;

unsigned SoundThread::ThreadProc(void* pThis)
{
	static_cast<SoundThread*>(pThis)->Loop();
	return 0;
}

void SoundThread::Start(Minima68Win* pEmulator)
{
	audioClient.Create();
	running = true;
	hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, ThreadProc, this, 0, nullptr));
	if (hThread == nullptr) {
		running = false;
	}
}

void SoundThread::Stop()
{
	if (running) {
		running = false;
		if (hThread != nullptr) {
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
			hThread = nullptr;
		}
	}
}

void SoundThread::Loop() const
{
	HRESULT result;
	ComPtr<IAudioRenderClient> pRenderClient;
	{
		IAudioRenderClient* p;
		result = audioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&p));
		if (FAILED(result)) {
			throw ApiException(result);
		}
		pRenderClient.Attach(p);
	}
	UINT32 bufferSize;
	result = audioClient->GetBufferSize(&bufferSize);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	result = audioClient->Start();
	if (FAILED(result)) {
		throw ApiException(result);
	}
	while (running) {
		UINT32 padding;
		result = audioClient->GetCurrentPadding(&padding);
		if (FAILED(result)) {
			throw ApiException(result);
		}
		auto framesToWrite = bufferSize - padding;
		if (framesToWrite > 0) {
			float* pBuffer;
			result = pRenderClient->GetBuffer(framesToWrite, reinterpret_cast<BYTE**>(&pBuffer));
			if (FAILED(result)) {
				throw ApiException(result);
			}
			auto frameCount = framesToWrite;
			while (frameCount > 0) {
				float sum = 0;
				for (auto& channel : ToneChannels) {
					sum += channel.Sample();
				}
				sum += EffectChannel.Sample();
				auto average = sum / (ToneChannelCount + 1);
				*pBuffer++ = average;
				*pBuffer++ = average;
				--frameCount;
			}
			result = pRenderClient->ReleaseBuffer(framesToWrite, 0);
			if (FAILED(result)) {
				throw ApiException(result);
			}
		}
		Sleep(0);
	}
	result = audioClient->Stop();
	if (FAILED(result)) {
		throw ApiException(result);
	}
}


inline float ToFloat(uint8_t b)
{
	return (static_cast<int>(b) - 128) / 128.0f;
}



void ToneChannel::UpdateSamples()
{
	if (pSourceSamples != nullptr) {
		for (int i = 0; i < ToneSampleCount; ++i) {
			samples[i] = ToFloat(pSourceSamples[i]);
		}
	}
	else {
		ZeroMemory(samples, sizeof(samples));
	}
}

void ToneChannel::SetSourceSamples(const uint8_t* pSamples)
{
	this->pSourceSamples = pSamples;
	sourceChanged = true;
}

void ToneChannel::SetVolume(uint8_t volume)
{
	this->volume = std::min(static_cast<int>(volume), MaxVolume);
}

void ToneChannel::SetFrequency(uint16_t frequency)
{
	step = static_cast<double>(frequency * ToneSampleCount) / SoundThread.SampleRate();
}

float ToneChannel::Sample()
{
	if (sourceChanged) {
		sourceChanged = false;
		UpdateSamples();
	}
	int index = static_cast<int>(phase);
	auto sample = samples[index % ToneSampleCount] * volume / MaxVolume;
	phase += step;
	if (phase >= ToneSampleCount) {
		phase -= ToneSampleCount;
	}
	return sample;
}

void EffectChannel::UpdateSamples()
{
	if (pSourceSamples != nullptr) {
		for (int i = 0; i < EffectSampleCount; ++i) {
			samples[i] = ToFloat(pSourceSamples[i]);
		}
	}
	else {
		ZeroMemory(samples, sizeof(samples));
	}
}

void EffectChannel::SetSourceSamples(const uint8_t* pSamples)
{
	this->pSourceSamples = pSamples;
	sourceChanged = true;
}

void EffectChannel::SetVolume(uint8_t volume)
{
	this->volume = std::min(static_cast<int>(volume), MaxVolume);
	if ((volume & 0x80) != 0) {
		phase = 0;
	}
}

void EffectChannel::SetRate(uint8_t rate)
{
	if (rate == 0) {
		step = 0;
		volume = 0;
	}
	else {
		step = static_cast<double>(255 * EffectSampleCount) / rate / SoundThread.SampleRate();
	}
}

float EffectChannel::Sample()
{
	if (sourceChanged) {
		sourceChanged = false;
		UpdateSamples();
	}
	if (phase < EffectSampleCount) {
		int index = static_cast<int>(phase);
		auto sample = samples[index % EffectSampleCount] * volume / MaxVolume;
		phase += step;
		if (phase >= EffectSampleCount) {
			phase -= EffectSampleCount;
		}
		return sample;
	}
	return 0;
}
