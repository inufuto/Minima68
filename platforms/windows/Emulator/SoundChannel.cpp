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


void ToneChannel::SetVolume(uint8_t volume)
{
	this->volume = std::min(static_cast<int>(volume), MaxVolume);
}

void ToneChannel::SetFrequency(uint16_t frequency)
{
	phaseDelta = (static_cast<uint64_t>(frequency) << 32) / SoundThread.SampleRate();
}

float ToneChannel::Sample()
{
	if (volume == 0 || phaseDelta == 0) {
		return 0.0f;
	}

	phase += phaseDelta;

	// Top bit indicates high/low half-cycle of a 50% duty square wave.
	float sample = ((phase & 0x80000000u) != 0) ? 1.0f : -1.0f;
	return sample * volume / MaxVolume;
}

void EffectChannel::SetVolume(uint8_t volume)
{
	this->volume = std::min(static_cast<int>(volume), MaxVolume);
	if ((volume & 0x80) != 0) {
		phase = 0;
		noiseState = 1;  // Reset LFSR
	}
}

void EffectChannel::SetFrequency(uint16_t frequency)
{
	phaseDelta = (static_cast<uint64_t>(frequency) << 32) / SoundThread.SampleRate();
}

float EffectChannel::Sample()
{
	if (volume > 0 && phaseDelta != 0) {
		const uint32_t previousPhase = phase;
		phase += phaseDelta;

		// Advance LFSR at configured frequency.
		if (phase < previousPhase) {
			const uint32_t lsb = noiseState & 1;
			noiseState >>= 1;
			if (lsb != 0) {
				noiseState ^= 0xB800;  // Feedback polynomial
			}
		}

		const float noise = (noiseState & 1) != 0 ? 1.0f : -1.0f;
		return noise * volume / MaxVolume;
	}
	return 0.0f;
}
