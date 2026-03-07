#include <windows.h>
#include "ScreenWindow.h"
#include "Direct2DBrush.h"
#include "EmulatorWindow.h"
#include "resource.h"

LRESULT ScreenWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnWmCreate(wParam, lParam);
	case WM_PAINT:
		return OnWmPaint(wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnWmLButtonDown(wParam, lParam);
	case WM_SETFOCUS:
		return OnWmSetFocus(wParam, lParam);
	}
	return RenderTargetWindow::OnMessage(message, wParam, lParam);
}

void ScreenWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	RenderTargetWindow::OnCreate(pCreateStruct);
	textFormat.Create("Consolas", 24);
	brush.Create(RenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	bitmap.CreatePng(RenderTarget(), ID_ADD);
}

void ScreenWindow::OnRender(::RenderTarget& renderTarget)
{
	RenderTargetWindow::OnRender(renderTarget);
	auto rect = D2D1::RectF(100.0f, 0.0f, 200.0f, 100.0f);
	renderTarget.DrawBitmap(bitmap, rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void ScreenWindow::OnSetFocus(HWND hOldWnd)
{
	SendMessage(GetParent(), WM_SELECT_PANE, 0, reinterpret_cast<LPARAM>(HWnd()));
}
