#include "Minima68Win.h"

#include <string>

uint8_t Minima68Win::ram[0x10000];
void Minima68Win::WriteMemory(uint16_t address, uint8_t value)
{
	ram[address] = value;
	if (address == 0xff00) {
		char c = static_cast<char>(value);
		if (c == '\r') {
			OutputDebugStringA("\n");
		}
		else if (c >= 32 && c <= 126) {
			OutputDebugStringA(std::string(1, c).c_str());
		}
	}
}

void Minima68Win::Start()
{
	mode = Normal;
	masterClock.AddDestination(&Cpu());
#ifdef _DEBUG
	LoadProgramFromFile(0x100, "D:\\8bit\\Minima68\\test\\test.bin");
	Cpu().Breakpoints().push_back(0x0100);
#endif
	Reset();
	masterClock.Start();
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

void Minima68Win::LoadProgramFromFile(uint16_t address, const char* path) {
	FILE* file;
	fopen_s(&file, path, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);
		fseek(file, 0, SEEK_SET);
		fread(ram + address, 1, size, file);
		fclose(file);
	}
}
#endif