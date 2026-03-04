#include "TitledPane.h"

LRESULT TitledPane::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnWmCreate(wParam, lParam);
	case WM_SIZE:
		return OnWmSize(wParam, lParam);
	case WM_ERASEBKGND:
		return OnWmEraseBackground(wParam, lParam);
	case WM_PAINT:
		return OnWmPaint(wParam, lParam);
	}
	return Window::OnMessage(message, wParam, lParam);
}

void TitledPane::OnCreate(CREATESTRUCT* pCreateStruct)
{
	Window::OnCreate(pCreateStruct);
	active = false;
	titleHeight = GetSystemMetrics(SM_CYSMCAPTION);
	font.Create(titleHeight * 2 / 3);
	pChild->Create(HWnd(), 1);
}

void TitledPane::OnSize(UINT width, UINT height)
{
	pChild->Move(0, titleHeight, width, height - titleHeight);
}

void TitledPane::OnEraseBackground(DeviceContext& dc)
{
	RECT rect;
	GetClientRect(HWnd(), &rect);
	HBRUSH hBrush = active ? GetSysColorBrush(COLOR_ACTIVECAPTION) : GetSysColorBrush(COLOR_INACTIVECAPTION);
	FillRect(dc.Hdc(), &rect, hBrush);
}

void TitledPane::OnPaint(DeviceContext& dc)
{
	auto text = GetText();
	RECT rect;
	GetClientRect(HWnd(), &rect);
	rect.bottom = rect.top + titleHeight;
	auto oldColor = dc.SetTextColor(GetSysColor(active ? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT));
	auto oldBkMode = dc.SetBackgroundMode(TRANSPARENT);
	auto hOldFont = dc.Select(font);
	dc.DrawText(text.c_str(), rect, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	dc.Select(hOldFont);
	dc.SetBackgroundMode(oldBkMode);
	dc.SetTextColor(oldColor);
}

void TitledPane::Active(bool active)
{
	this->active = active;
	Invalidate();
	if (pChild->HWnd() != nullptr) {
		SetFocus(pChild->HWnd());
	}
}
