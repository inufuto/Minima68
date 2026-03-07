#pragma once

#include "MasterClock.h"
#include "../../../core/Minima68.h"

class Minima68Win : public Minima68, public Debugger
{
private:
	MasterClock masterClock;
	static uint8_t ram[0x10000];
protected:
	uint8_t ReadMemory(uint16_t address) override { return ram[address]; }
	void WriteMemory(uint16_t address, uint8_t value) override;
public:
	explicit Minima68Win(MasterClock::Owner* pOwner) : Minima68(this), masterClock(pOwner, MasterClockFrequency) {}
	void Start();
	void Stop() { masterClock.Stop(); }
	void Pause() override { masterClock.Pause(); }
#ifdef _DEBUG
	void LoadProgramFromFile(const char* path);
#endif
	static void LoadProgramFromFile(uint16_t address, const char* path);
};
