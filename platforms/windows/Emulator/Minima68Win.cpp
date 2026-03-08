#include "Minima68Win.h"

#include <string>

void Minima68Win::Start()
{
	mode = Normal;
	primaryClock.AddDestination(&Cpu());
#ifdef _DEBUG
	ZeroMemory(Ram(), 0xff00);
	LoadProgramFromFile(0x100, "D:\\8bit\\Minima68\\test\\test.bin");
#endif
	Reset();
	primaryClock.Start();
}

#ifdef _DEBUG
bool Minima68Win::PauseRequired(const ::Cpu* pCpu, uint16_t address)
{
	switch (mode) {
	case Step:
		if (address != lastInstructionAddress) {
			mode = Normal;
			return true;
		}
		break;
	case Next:
		if (address == nextProgramCounter) {
			mode = Normal;
			return true;
		}
		break;
	}
	return Debugger::PauseRequired(pCpu, address);
}

void Minima68Win::ExecuteStep()
{
	lastInstructionAddress = Cpu().CurrentInstructionAddress();
	mode = Step;
	Resume();
}

void Minima68Win::ExecuteToNext()
{
	nextProgramCounter = Cpu().CurrentInstructionAddress() + Cpu().CurrentInstructionSize();
	mode = Next;
	Resume();
}

void Minima68Win::SetColor(int index, uint8_t r, uint8_t g, uint8_t b) {
	assert(index >= 0 && index < ColorCount);
	colors[index] = { b, g, r, 0xff };
}

void Minima68Win::LoadProgramFromFile(uint16_t address, const char* path) {
	FILE* file;
	fopen_s(&file, path, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);
		fseek(file, 0, SEEK_SET);
		fread(Ram() + address, 1, size, file);
		fclose(file);
	}
}
#endif