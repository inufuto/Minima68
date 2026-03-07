#pragma once
#include "../WinApi/ApplicationWindow.h"
#include "../../../core/Cpu.h"

static constexpr UINT WM_UPDATE_EMULATOR = WM_APP + 1;
static constexpr UINT WM_SELECT_PANE = WM_APP + 2;

class EmulatorWindow : public ApplicationWindow
{
protected:
	using ApplicationWindow::ApplicationWindow;

	bool AddBreakpoint(BreakpointHolder& holder);
public:
	~EmulatorWindow() override = default;
};
