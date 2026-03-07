#include "EmulatorWindow.h"

#include "BreakpointDialog.h"

bool EmulatorWindow::AddBreakpoint(BreakpointHolder& holder)
{
	BreakpointDialog dialog;
	if (dialog.ShowModal(HWnd()) == IDOK) {
		holder.AddBreakpoint(dialog.Address());
		return true;
	}
	return false;
}
