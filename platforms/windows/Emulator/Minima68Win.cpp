#include "Minima68Win.h"

#include <string>

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


Minima68Win::Minima68Win(PrimaryClock::Owner* pOwner): 
	Minima68(this), primaryClock(pOwner, PrimaryClockFrequency),
channel(Lead2Wave)
{}

void Minima68Win::Start()
{
	mode = Normal;
	primaryClock.AddDestination(&Cpu());
	channel.Start();
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