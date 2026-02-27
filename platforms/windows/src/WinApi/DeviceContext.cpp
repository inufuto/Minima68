#include "DeviceContext.h"

DeviceContext::DeviceContext(HWND hWnd): hWnd(hWnd), hDC(::GetDC(hWnd)) {}

DeviceContext::~DeviceContext()
{
	if (hWnd != nullptr && hDC != nullptr) {
		ReleaseDC(hWnd, hDC);
	}	
}
