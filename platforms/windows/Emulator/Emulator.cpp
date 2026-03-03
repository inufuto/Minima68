#include "Emulator.h"

#include <string>

void Emulator::WritePort(uint16_t address, uint8_t value)
{
	char c = static_cast<char>(value);
	if (c == '\r') {
		OutputDebugStringA("\n");
	}
	else if (c >= 32 && c <= 126) {
		OutputDebugStringA(std::string(1, c).c_str());
	}
}

void Emulator::Start(HWND hWnd)
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
void Emulator::LoadProgramFromFile(uint16_t address, const char* path) {
	FILE* file;
	fopen_s(&file, path, "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		auto size = ftell(file);
		fseek(file, 0, SEEK_SET);
		fread(Bytes() + address, 1, size, file);
		fclose(file);
	}
}
#endif