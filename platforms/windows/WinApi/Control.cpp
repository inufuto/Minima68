#include "Control.h"

LRESULT Control::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(wndProc,HWnd(), message, wParam, lParam);
}

void Control::Subclass(HWND hWnd)
{
	Attach(hWnd);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	wndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
}
