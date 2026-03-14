#include <mmdeviceapi.h>
#include "WindowsAudioClient.h"

void WindowsAudioClient::Create()
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	ComPtr<IMMDeviceEnumerator> pEnumerator;
	pEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));

	ComPtr<IMMDevice> pDevice;
	auto result = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	if (FAILED(result)) {
		throw ApiException(result);
	}

	IAudioClient* p;
	result = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&p);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	pAudioClient.Attach(p);

	WAVEFORMATEX* pWaveFormat;
	result = pAudioClient->GetMixFormat(&pWaveFormat);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	sampleRate = pWaveFormat->nSamplesPerSec;
	
	WAVEFORMATEXTENSIBLE waveFormat;
	CopyMemory(&waveFormat.Format, pWaveFormat, sizeof(WAVEFORMATEXTENSIBLE));
	waveFormat.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE;
	waveFormat.Format.nChannels = 2;
	waveFormat.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

	result = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, REFTIMES_PER_SEC / 100, 0, &waveFormat.Format, nullptr);
	if (FAILED(result)) {
		throw ApiException(result);
	}
}
