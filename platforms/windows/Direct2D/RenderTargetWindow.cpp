#include "RenderTargetWindow.h"

LRESULT RenderTargetWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
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

void RenderTargetWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	Window::OnCreate(pCreateStruct);
	renderTarget.Create(HWnd());
}

void RenderTargetWindow::OnSize(UINT width, UINT height)
{
	if (width == 0 || height == 0) {
		return;
	}
	renderTarget->Resize(D2D1::SizeU(width, height));
}

void RenderTargetWindow::OnPaint(DeviceContext& dc)
{
	renderTarget->BeginDraw();
	OnRender(renderTarget);
	renderTarget->EndDraw();
}

float RenderTargetWindow::GetDpiScale() const
{
	auto dpi = ::GetDpiForWindow(HWnd());
	return dpi / 96.0f;
}

UINT RenderTargetWindow::DipToPixel(float dip) const
{
	return static_cast<UINT>(dip * GetDpiScale());
}
