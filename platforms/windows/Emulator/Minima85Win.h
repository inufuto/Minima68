#pragma once

#include "MasterClock.h"
#include "../../../core/Minima85.h"

class Minima85Win : public Minima85
{
private:
	MasterClock masterClock;
	static uint8_t ram[0x10000];
protected:
	uint8_t ReadMemory(uint16_t address) override { return ram[address]; }
	void WriteMemory(uint16_t address, uint8_t value) override { ram[address] = value; }
	uint8_t ReadPort(uint16_t address) override { return 0; }
	void WritePort(uint16_t address, uint8_t value) override;
public:
	explicit Minima85Win(MasterClock::Owner* pOwner) : masterClock(pOwner, MasterClockFrequency) {}
	void Start();
	static void LoadProgramFromFile(uint16_t address, const char* path);
	void Stop() { masterClock.Stop(); }
#ifdef _DEBUG
	void LoadProgramFromFile(const char* path);
#endif
};
