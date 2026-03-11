#include <cstring>
#include "Minima68.h"

#include "Joystick.h"
#include "MemoryMap.h"
#include "Sound.h"
#include "Video.h"

const uint8_t PianoWave[] = {
	254,238,204,172,150,128,104,84,
	74,65,50,36,32,33,25,9,
	0,9,25,33,32,36,50,65,
	74,84,104,128,150,172,204,238,
};
const uint8_t Lead2Wave[] = {
	203,160,75,22,17,23,17,10,
	11,10,6,4,4,3,0,0,
	2,0,0,3,4,4,6,10,
	11,10,17,23,17,22,75,160,
};
const uint8_t BassWave[] = {
	254,245,219,183,145,110,81,56,
	35,18,5,0,5,18,35,50,
	55,50,35,18,5,0,5,18,
	35,56,81,110,145,183,219,245,
};

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
	else if (address >= VolumeAddress && address < VolumeAddress + ToneChannelCount * 2) {
		if ((address - VolumeAddress) % 2 == 0) {
			auto index = address - VolumeAddress;
			auto volume = pOwner->Ram()[address];
			pOwner->SetToneVolume(index, volume);
		}
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

	WriteMemory(0xfffe, HighByte(StartAddress));
	WriteMemory(0xffff, LowByte(StartAddress));
		memcpy(Ram() + 0x100, TestCode, 0x2000);
	//memcpy(Ram() + ShortWaveAddress, PianoWave, 32);
	//memcpy(Ram() + ShortWaveAddress + 32, Lead2Wave, 32);
	//memcpy(Ram() + ShortWaveAddress + 64, BassWave, 32);
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
