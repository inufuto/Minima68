#include <cstring>

#include "Minima68.h"
#include "Joystick.h"
#include "MemoryMap.h"
#include "Sound.h"
#include "Video.h"

extern const uint8_t TestCode[];

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
	if (address >= ToneSampleAddress && address < ToneSampleAddress + ToneChannelCount * 2) {
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
			auto index = address - ToneVolumeAddress;
			auto volume = pOwner->Ram()[address];
			pOwner->SetToneVolume(index, volume);
		}
	}
	else if (address >= EffectSampleAddress && address <EffectSampleAddress + 2)
	{
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
	//static const uint8_t PaletteValues[] = {
	//	0x00, 0x00, 0x00, 0x55, 0x00, 0xff, 0xff, 0x00,
	//	0x00, 0xff, 0xbb, 0xff, 0x00, 0xff, 0x00, 0x00,
	//	0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff,
	//	0x00, 0x00, 0x00, 0x99, 0x44, 0x44, 0xff, 0xbb,
	//	0x55, 0xbb, 0x55, 0x55, 0x00, 0x55, 0x00, 0x00,
	//	0x55, 0xbb, 0xff, 0xff, 0x55, 0xbb, 0xbb, 0xbb,
	//};
	//{
	//	auto pSource = PaletteValues;
	//	for (auto i = 0; i < ColorCount; ++i) {
	//		auto r = *pSource++;
	//		auto g = *pSource++;
	//		auto b = *pSource++;
	//		SetColor(i, r, g, b);
	//	}
	//}
	ram[ScrollXAddress] = ram[ScrollYAddress] = 0;
	ram[JoystickAddress] = 0;
	for (auto i = 0; i < ToneChannelCount; ++i) {
		SetToneVolume(i, 0);
	}
	SetEffectVolume(0);

	WriteMemory(0xfffe, HighByte(StartAddress));
	WriteMemory(0xffff, LowByte(StartAddress));
		//memcpy(Ram() + 0x100, TestCode, 0x2000);
	cpu.Reset();
}

uint8_t Minima68::ScrollX() const
{
	return ram[ScrollXAddress];
}

uint8_t Minima68::ScrollY() const
{
	return ram[ScrollYAddress];
}
