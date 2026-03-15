#include <cstring>

#include "Minima68.h"
#include "Input.h"
#include "MemoryMap.h"
#include "Sound.h"
#include "Video.h"

//extern const uint8_t TestCode[];

inline uint16_t LoadWord(const uint8_t* p)
{
	return MakeWord(p[0], p[1]);
}

uint8_t Minima68::Memory::Read(const uint16_t address) const
{
	uint8_t value = pOwner->ReadMemory(address);
	if (address == JoystickAddress) {
		value &= Joystick::All;
	}
	else if (address == KeyCodeAddress) {
		return pOwner->RetrieveKeyCode();
	}
	return value;
}

void Minima68::Memory::Write(const uint16_t address, const uint8_t value)
{
	pOwner->WriteMemory(address, value);
	if (address >= PaletteAddress && address < PaletteAddress + ColorCount * 3) {
		auto index = (address - PaletteAddress) / 3;
		auto p = pOwner->Ram() + PaletteAddress + index * 3;
		auto r = *p++;
		auto g = *p++;
		auto b = *p;
		pOwner->SetColor(index, r, g, b);
	}
	else if (address == PageAddress) {
		pOwner->SetPage(value & 0x01);
	}
	else if (address >= ToneSampleAddress && address < ToneSampleAddress + ToneChannelCount * 2) {
		auto index = (address - ToneSampleAddress) / 2;
		auto sampleAddress = LoadWord(pOwner->Ram() + ToneSampleAddress + index * 2);
		pOwner->SetToneSample(index, pOwner->Ram() + sampleAddress);
	}
	else if (address >= FrequencyAddress && address < FrequencyAddress + ToneChannelCount * 2) {
		auto index = (address - FrequencyAddress) / 2;
		auto frequency = LoadWord(pOwner->Ram() + FrequencyAddress + index * 2);
		pOwner->SetToneFrequency(index, frequency);
	}
	else if (address >= ToneVolumeAddress && address < ToneVolumeAddress + ToneChannelCount * 2) {
		if ((address - ToneVolumeAddress) % 2 == 0) {
			auto index = (address - ToneVolumeAddress) /2;
			auto volume = pOwner->Ram()[address];
			pOwner->SetToneVolume(index, volume);
		}
	}
	else if (address >= EffectSampleAddress && address < EffectSampleAddress + 2) {
		auto sampleAddress = LoadWord(pOwner->Ram() + EffectSampleAddress);
		pOwner->SetEffectSample(pOwner->Ram() + sampleAddress);
	}
	else if (address == EffectRateAddress) {
		auto rate = pOwner->Ram()[address];
		pOwner->SetEffectRate(rate);
	}
	else if (address == EffectVolumeAddress) {
		auto volume = pOwner->Ram()[address];
		pOwner->SetEffectVolume(volume);
	}
}

void Minima68::Reset()
{
	ram[JoystickAddress] = 0;
	SetPage(0);
	for (auto i = 0; i < ToneChannelCount; ++i) {
		SetToneVolume(i, 0);
	}
	SetEffectVolume(0);

	WriteMemory(0xfffe, HighByte(StartAddress));
	WriteMemory(0xffff, LowByte(StartAddress));
	cpu.Reset();
}

char Minima68::RetrieveKeyCode() {
	if (keyCodes.empty()) {
		return 0;
	}
	char charCode = keyCodes.front();
	keyCodes.pop();
	return charCode;
}
