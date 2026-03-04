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
	EmulatorWindow(::LoadMenu(HInstance, MAKEINTRESOURCE(IDR_MAIN)),
		LoadAccelerators(HInstance, MAKEINTRESOURCE(IDR_MAIN))),
	emulator(this), registerWindow(&emulator.Cpu()), subWindow(emulator)
{
}

void MainWindow::Invalidate()
{
	if (!waitingForUpdate) {
		waitingForUpdate = true;
		PostMessage(HWnd(), WM_UPDATE_EMULATOR, 0, 0);
	}
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
	case WM_UPDATE_EMULATOR:
		registerWindow.Invalidate();
		memoryWindow.Invalidate();
		subWindow.Invalidate();
		//Invalidate();
		//RedrawWindow(HWnd(), nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN);
		UpdateWindow(HWnd());
		waitingForUpdate = false;
		return 0;
	}
	return EmulatorWindow::OnMessage(message, wParam, lParam);
}

void MainWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	waitingForUpdate = false;
	EmulatorWindow::OnCreate(pCreateStruct);
	SetText(ProductName);
	registerPane.Create(HWnd(), 1);
	registerPane.SetText("Registers");
	memoryPane.Create(HWnd(), 2);
	memoryPane.SetText("Memory");
	subWindow.Create(HWnd(), 3);
	SolidColorBrush brush;
	brush.Create(subWindow.RenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	emulator.Start();
}

void MainWindow::OnDestroy()
{
	emulator.Stop();
	EmulatorWindow::OnDestroy();
}

void MainWindow::OnSize(UINT width, UINT height)
{
	auto x = 0;
	{
		auto paneWidth = registerWindow.MinWindowWidth();
		registerPane.Move(x, 0, paneWidth, height);
		x += paneWidth;
	}
	{
		auto paneWidth = memoryWindow.MinWindowWidth();
		memoryPane.Move(x, 0, paneWidth, height);
		x += paneWidth;
	}
	subWindow.Move(x, 0, width - x, height);
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
