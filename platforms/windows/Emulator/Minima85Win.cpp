#include "Minima85Win.h"

#include <string>

uint8_t Minima85Win::ram[0x10000];

void Minima85Win::WritePort(uint16_t address, uint8_t value)
{
	char c = static_cast<char>(value);
	if (c == '\r') {
		OutputDebugStringA("\n");
	}
	else if (c >= 32 && c <= 126) {
		OutputDebugStringA(std::string(1, c).c_str());
	}
}

void Minima85Win::Start(HWND hWnd)
{
	masterClock.AddDestination(&Cpu());
#ifdef _DEBUG
	LoadProgramFromFile(0, "D:\\8bit\\Minima85\\test\\test.bin");
	LoadProgramFromFile(0x100,"D:\\8bit\\Minima85\\8080EXM.COM");
#endif
	Cpu().Reset();
	masterClock.Start(hWnd);
}

#ifdef _DEBUG
void Minima85Win::LoadProgramFromFile(uint16_t address, const char* path) {
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