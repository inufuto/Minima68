#include <windows.h>

#include "MainWindow.h"

#include <string>

#include "Direct2DBrush.h"
#include "resource.h"

extern HINSTANCE HInstance;
extern LPCSTR ProductName;

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

void SubWindow::OnRender(::RenderTarget& renderTarget)
{
	RenderTargetWindow::OnRender(renderTarget);
	auto s = std::to_string(emulator.Cpu().ProgramCounter());
	renderTarget.DrawText(s.c_str(), textFormat.Ptr(), D2D1::RectF(0.0f, 0.0f, 200.0f, 100.0f), brush.Ptr());
	auto rect = D2D1::RectF(100.0f, 0.0f, 200.0f, 100.0f);
	renderTarget.DrawBitmap(bitmap, rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}


MainWindow::MainWindow() :
	ApplicationWindow(::LoadMenu(HInstance, MAKEINTRESOURCE(IDR_MAIN)),
		LoadAccelerators(HInstance, MAKEINTRESOURCE(IDR_MAIN))),
	emulator(this),	registerWindow(&emulator.Cpu()), subWindow(emulator)
{
}

void MainWindow::Invalidate()
{
	registerWindow.Invalidate();
	subWindow.Invalidate();
}

LRESULT MainWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnWmCreate(wParam, lParam);
	case WM_DESTROY:
		return OnWmDestroy(wParam, lParam);
	case WM_SIZE:
		return OnWmSize(wParam, lParam);
	case WM_PAINT:
		return OnWmPaint(wParam, lParam);
	}
	return ApplicationWindow::OnMessage(message, wParam, lParam);
}

void MainWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	SetText(ProductName);
	titledPane.Create(HWnd(), 1);
	titledPane.SetText("Registers");
	titledPane.Active(true);
	ApplicationWindow::OnCreate(pCreateStruct);
	subWindow.Create(HWnd(), 2);
	SolidColorBrush brush;
	brush.Create(subWindow.RenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	emulator.Start();
}

void MainWindow::OnDestroy()
{
	emulator.Stop();
	ApplicationWindow::OnDestroy();
}

void MainWindow::OnSize(UINT width, UINT height)
{
	auto w = registerWindow.MinWindowWidth();
	titledPane.Move(0, 0, w, height);
	subWindow.Move(w, 0, width - w, height);
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
