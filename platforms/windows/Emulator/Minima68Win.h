#pragma once

#include "MasterClock.h"
#include "../../../core/Minima68.h"

class Minima68Win : public Minima68, public Debugger
{
private:
	MasterClock masterClock;
	enum Mode
	{
		Normal, Step, Next
	} mode;
	uint16_t lastInstructionAddress;
	uint16_t nextProgramCounter;
public:
	explicit Minima68Win(MasterClock::Owner* pOwner) : Minima68(this), masterClock(pOwner, MasterClockFrequency) {}
	void Start();
	void Stop() { masterClock.Stop(); }
	void Pause() override { masterClock.Pause(); }
	void Resume() { masterClock.Resume(); }
	bool Paused() const { return masterClock.Paused(); }
	bool PauseRequired(const ::Cpu* pCpu, uint16_t address) override;
	void ExecuteStep();
	void ExecuteToNext();
#ifdef _DEBUG
	void LoadProgramFromFile(const char* path);
#endif
	static void LoadProgramFromFile(uint16_t address, const char* path);
};
