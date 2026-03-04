#pragma once

#include <windows.h>
#include "../../../core/ClockSource.h"

class MasterClock : public ClockSource
{
public:
	class Owner
	{
	public:
		virtual void Invalidate() = 0;
	};
private:
	double targetFrequency;
private:
	Owner* pOwner;
	LARGE_INTEGER timerFrequency;
	bool running;
	LARGE_INTEGER last;
	int32_t time;
	HANDLE hThread;
private:
	static unsigned __stdcall ThreadProc(void* pThis);
public:
	explicit MasterClock(Owner* pOwner, double targetFrequency);
	~MasterClock() override;
	int32_t Time() const { return time; }
	void Start();
	void Stop();
	void Loop();
};