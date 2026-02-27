#include <windows.h>

#include "MainWindow.h"

#include "Direct2DBrush.h"
#include "resource.h"

extern HINSTANCE HInstance;

LRESULT SubWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnWmCreate(wParam, lParam);
	case WM_PAINT:
		return OnWmPaint(wParam, lParam);
	}
	return RenderTargetWindow::OnMessage(message, wParam, lParam);
}

void SubWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	RenderTargetWindow::OnCreate(pCreateStruct);
	textFormat.Create("Consolas", 24);
	brush.Create(RenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	bitmap.CreatePng(RenderTarget(), ID_ADD);
}

void SubWindow::OnRender(class ::RenderTarget& renderTarget)
{
	RenderTargetWindow::OnRender(renderTarget);
	renderTarget.DrawText("AAA", textFormat, D2D1::RectF(0.0f, 0.0f, 100.0f, 100.0f), brush);
	renderTarget.DrawBitmap(bitmap, D2D1::RectF(100.0f, 0.0f, 200.0f, 100.0f), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}


MainWindow::MainWindow()
	: ApplicationWindow(::LoadMenu(HInstance, MAKEINTRESOURCE(IDR_MAIN))
		, LoadAccelerators(HInstance, MAKEINTRESOURCE(IDR_MAIN)))
{}

LRESULT MainWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnWmCreate(wParam, lParam);
	case WM_SIZE:
		return OnWmSize(wParam, lParam);
	case WM_PAINT:
		return OnWmPaint(wParam, lParam);
	}
	return ApplicationWindow::OnMessage(message, wParam, lParam);
}

void MainWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	ApplicationWindow::OnCreate(pCreateStruct);
	subWindow.Create(HWnd(), 1);
	SolidColorBrush brush;
	brush.Create(subWindow.RenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
}

void MainWindow::OnSize(UINT width, UINT height)
{
	ApplicationWindow::OnSize(width, height);
	subWindow.Move(0, 20, width, height - 20);
}

void MainWindow::OnCommand(UINT id, UINT notificationCode, HWND hWnd)
{
	switch (id) {
	case ID_FILE_EXIT:
		SendMessage(HWnd(), WM_CLOSE, 0, 0);
	}
}

void MainWindow::OnPaint(DeviceContext& dc)
{
	dc.TextOut(0, 0, "Hello, World!", 13);
}
