#include <process.h>

#include "SoundChannel.h"
#include "Minima68Win.h"

ToneChannel ToneChannels[ToneChannelCount];
class SoundThread SoundThread;

unsigned SoundThread::ThreadProc(void* pThis)
{
	static_cast<SoundThread*>(pThis)->Loop();
	return 0;
}

void SoundThread::Start(Minima68Win* pEmulator)
{
	audioClient.Create();
	//auto pSourceSamples = pEmulator->Ram() + ShortWaveAddress;
	//for (auto& channel : ToneChannels) {
	//	channel.SourceSamples(pSourceSamples);
	//	pSourceSamples += ToneSampleCount;
	//}
	//ToneChannels[0].SetFrequency(440);
	//ToneChannels[1].SetFrequency(440 * 2);
	//ToneChannels[2].SetFrequency(440 / 2);

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

void ToneChannel::SetFrequency(uint16_t frequency)
{
	this->frequency = frequency;
	step = static_cast<double>(frequency * ToneSampleCount) / SoundThread.SampleRate();
}

float ToneChannel::Sample()
{
	if (sourceChanged) {
		sourceChanged = false;
		UpdateSamples();
	}
	auto sample = samples[static_cast<int>(phase)] * volume / MaxVolume;
	phase += step;
	if (phase >= ToneSampleCount) {
		phase -= ToneSampleCount;
	}
	return sample;
}
