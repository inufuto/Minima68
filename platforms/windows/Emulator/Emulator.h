#pragma once

#include "MasterClock.h"
#include "../../../core/EmulatorCore.h"

class Emulator : public EmulatorCore
{
public:
	static constexpr double TargetFrequency = 10000000;
private:
	MasterClock masterClock;
protected:
	void WritePort(uint16_t address, uint8_t value) override;
public:
	Emulator() : masterClock(TargetFrequency) {}
	void Start(HWND hWnd);
	static void LoadProgramFromFile(uint16_t address, const char* path);
	void Stop() { masterClock.Stop(); }
#ifdef _DEBUG
	void LoadProgramFromFile(const char* path);
#endif
};
