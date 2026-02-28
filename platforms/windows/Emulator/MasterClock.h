#pragma once

#include <windows.h>
#include "../../../core/ClockSource.h"

class MasterClock : public ClockSource
{
private:
	double targetFrequency;
private:
	LARGE_INTEGER timerFrequency;
	HWND hWnd;
	bool running;
	LARGE_INTEGER last;
	int32_t time;
	HANDLE hThread;
public:
	explicit MasterClock(double targetFrequency);
	~MasterClock() override;
	int32_t Time() const { return time; }
	void Start(HWND hWnd);
	void Stop() { running = false; }
	void Loop();
};