#pragma once
#include <windows.h>

#include "Font.h"
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

	void Select(const Font& font) const { SelectObject(hDC, font.HFont()); }
	void GetTextMetrics(TEXTMETRIC* pTextMetric) const { ::GetTextMetrics(hDC, pTextMetric); }
	void TextOut(int x, int y, LPCSTR string, int count) const
	{
		::TextOut(hDC, x, y, string, count);
	}
	void DrawText(const char* string, const RECT& rect, UINT format) const
	{
		::DrawText(hDC, string, -1, const_cast<RECT*>(&rect), format);
	}
};
