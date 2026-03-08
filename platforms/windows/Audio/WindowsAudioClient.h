#pragma once
#include <audioclient.h>
#include "../../../core/Uncopyable.h"
#include "../WinApi/ComPtr.h"

class WindowsAudioClient : Uncopyable
{
private:
	static const REFERENCE_TIME REFTIMES_PER_SEC = 10000 * 1000;
	ComPtr<IAudioClient> pAudioClient;
	uint32_t sampleRate;
public:
	IAudioClient* Ptr() const { return pAudioClient; }
	IAudioClient* operator->() const { return pAudioClient; }
	uint32_t SampleRate() const { return sampleRate; }
	void Create();
};
