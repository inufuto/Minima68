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
	explicit DeviceContext(const HDC hDC) : hDC(hDC), hWnd(nullptr) {}
	explicit DeviceContext(HWND hWnd);
	~DeviceContext() override;
	HDC Hdc() const { return hDC; }

	auto Select(HGDIOBJ hObject) const { return SelectObject(hDC, hObject); }
	auto Select(const Font& font) const { return SelectObject(hDC, font.HFont()); }
	void GetTextMetrics(TEXTMETRIC* pTextMetric) const { ::GetTextMetrics(hDC, pTextMetric); }
	COLORREF SetTextColor(const COLORREF color) const { return ::SetTextColor(hDC, color); }
	auto SetBackgroundMode(int backgroundMode) const { return ::SetBkMode(hDC, backgroundMode); }
	void TextOut(int x, int y, LPCSTR string, int count) const
	{
		::TextOut(hDC, x, y, string, count);
	}
	void DrawText(const char* string, const RECT& rect, UINT format) const
	{
		::DrawText(hDC, string, -1, const_cast<RECT*>(&rect), format);
	}
};
