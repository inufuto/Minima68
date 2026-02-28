#include <process.h>
#include "MasterClock.h"

unsigned MasterClock::ThreadProc(void* pThis)
{
	static_cast<MasterClock*>(pThis)->Loop();
	return 0;
}

MasterClock::MasterClock(double targetFrequency) :targetFrequency(targetFrequency), running(false)
{
	QueryPerformanceFrequency(&timerFrequency);
}

MasterClock::~MasterClock() {
	if (hThread != nullptr) {
		CloseHandle(hThread);
		hThread = nullptr;
	}
}

void MasterClock::Start(HWND hWnd)
{
	this->hWnd = hWnd;
	QueryPerformanceCounter(&last);
	running = true;
	time = 0;
	hThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, ThreadProc, this, 0, nullptr));
	if (hThread == nullptr) {
		running = false;
	}
}

void MasterClock::Loop()
{
	while (running) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		auto elapsed = static_cast<double>(now.QuadPart - last.QuadPart) / static_cast<double>(timerFrequency.QuadPart);
		last = now;
		auto cyclesToRun = static_cast<uint32_t>(targetFrequency * elapsed);
		for (uint32_t i = 0; i < cyclesToRun; i++) {
			OnClock(time);
			++time;
		}
		InvalidateRect(hWnd, nullptr, TRUE);
		Sleep(0);
	}
}
