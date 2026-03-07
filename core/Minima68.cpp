#include "Minima68.h"

void Minima68::Reset()
{
	WriteMemory(0xfffe, HighByte(StartAddress));
	WriteMemory(0xffff, LowByte(StartAddress));
	cpu.Reset();
}
