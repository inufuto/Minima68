#include "SoundChannel.h"

#include <process.h>

float SoundChannel::ToFloat(uint8_t b)
{
	return (static_cast<int>(b) - 128) / 128.0f;
}

unsigned SoundChannel::ThreadProc(void* pThis)
{
	static_cast<SoundChannel*>(pThis)->Loop();
	return 0;
}

void SoundChannel::Start()
{
	audioClient.Create();
	running = true;
	hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, ThreadProc, this, 0, nullptr));
	if (hThread == nullptr) {
		running = false;
	}
}

void SoundChannel::Stop()
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

void SoundChannel::Loop()
{
	ComPtr<IAudioRenderClient> pRenderClient;
	{
		IAudioRenderClient* p;
		audioClient->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&p));
		pRenderClient.Attach(p);
	}
	UINT32 bufferSize;
	audioClient->GetBufferSize(&bufferSize);
	audioClient->Start();
	while (running) {
		UINT32 padding;
		audioClient->GetCurrentPadding(&padding);
		auto framesToWrite = bufferSize - padding;
		if (framesToWrite>0) {
			BYTE* pBuffer;
			pRenderClient->GetBuffer(framesToWrite, &pBuffer);
			Write(reinterpret_cast<float*>(pBuffer), framesToWrite);
			pRenderClient->ReleaseBuffer(framesToWrite, 0);
		}
		Sleep(0);
	}
}

void ToneChannel::UpdateSamples()
{
	for (int i = 0; i < SampleCount; ++i) {
		//samples[i] = (static_cast<int>(pSourceSamples[i]) - 128) / 128.0f;
		samples[i] = ToFloat(pSourceSamples[i]);
	}
}

void ToneChannel::Write(float* pBuffer, UINT32 framesToWrite)
{
	while (framesToWrite > 0) {
		auto sample = samples[static_cast<int>(phase)] * Volume();
		phase += step;
		if (phase >= SampleCount) {
			phase -= SampleCount;
		}
		*pBuffer++ = sample;
		*pBuffer++ = sample;
		--framesToWrite;
	}
}

void ToneChannel::Start()
{
	SoundChannel::Start();
	phase = 0;
	step = static_cast<double>(frequency * SampleCount) / SampleRate();
}
