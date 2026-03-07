#pragma once

#include "resource.h"
#include "../WinApi/Control.h"
#include "../WinApi/Dialog.h"

class BreakpointDialog : public Dialog
{
private:
	Edit addressEdit;
	Button okButton;
	uint16_t address;
private:
	void OnChangeAddress();
protected:
	void OnInitDialog() override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
public:
	BreakpointDialog() : Dialog(IDD_BREAKPOINT) {}
	uint16_t Address() const { return address; }
};
