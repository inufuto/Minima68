#include "Minima68.h"

uint8_t Minima68::ram[0x10000];

void Minima68::Reset()
{
	WriteMemory(0xfffe, HighByte(StartAddress));
	WriteMemory(0xffff, LowByte(StartAddress));
	cpu.Reset();
}
