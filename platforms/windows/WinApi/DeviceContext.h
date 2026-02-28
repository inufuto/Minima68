#pragma once
#include <windows.h>
#include "../../../core/Uncopyable.h"

class DeviceContext : public Uncopyable
{
private:
	HDC hDC;
	HWND hWnd;
public:
	DeviceContext(HDC hDC) : hDC(hDC), hWnd(nullptr) {}
	explicit DeviceContext(HWND hWnd);
	~DeviceContext();
	HDC Hdc() const { return hDC; }

	void TextOut(int x, int y, LPCSTR string, int count) const
	{
		::TextOut(hDC, x, y, string, count);
	}
};
