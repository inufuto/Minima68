#include <windows.h>

#include <string>
#undef min
#undef max
#include "MainWindow.h"
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
	auto rect = D2D1::RectF(100.0f, 0.0f, 200.0f, 100.0f);
	renderTarget.DrawBitmap(bitmap, rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}


MainWindow::MainWindow() :
	EmulatorWindow(::LoadMenu(HInstance, MAKEINTRESOURCE(IDR_MAIN)),
		LoadAccelerators(HInstance, MAKEINTRESOURCE(IDR_MAIN))),
	emulator(this), registerWindow(&emulator.Cpu()),
	assemblyWindow(&emulator.Cpu(), emulator.MemorySpaceAt(0)), subWindow(emulator)
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
	case WM_ERASEBKGND:
		return OnWmEraseBackground(wParam, lParam);
	case WM_UPDATE_EMULATOR:
		registerWindow.Invalidate();
		assemblyWindow.UpdateList();
		memoryWindow.Invalidate();
		subWindow.Invalidate();
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
	assemblyPane.Create(HWnd(), 2);
	assemblyPane.SetText("Assembly");
	memoryPane.Create(HWnd(), 3);
	memoryPane.SetText("Memory");
	subWindow.Create(HWnd(), 4);
	SolidColorBrush brush;
	brush.Create(subWindow.RenderTarget(), D2D1::ColorF(D2D1::ColorF::Red));
	emulator.Start();
	SetFocus(memoryWindow.HWnd());
}

void MainWindow::OnDestroy()
{
	emulator.Stop();
	EmulatorWindow::OnDestroy();
}

void MainWindow::OnSize(UINT width, UINT height)
{
	auto cxBorder = GetSystemMetrics(SM_CXBORDER);
	auto x = 0;
	{
		auto paneWidth = std::max(registerWindow.MinWindowWidth(), assemblyWindow.MinWindowWidth());
		{
			auto paneHeight = registerPane.TitleHeight() + registerWindow.MinWindowHeight();
			registerPane.Move(x, 0, paneWidth, paneHeight);
			assemblyPane.Move(x, paneHeight, paneWidth, height - paneHeight);
		}
		x += paneWidth + cxBorder;
	}
	{
		auto paneWidth = memoryWindow.MinWindowWidth();
		memoryPane.Move(x, 0, paneWidth, height);
		x += paneWidth + cxBorder;
	}
	subWindow.Move(x, 0, width - x, height);
}

void MainWindow::OnEraseBackground(DeviceContext& dc)
{
	RECT rect;
	GetClientRect(&rect);
	dc.FillRect(&rect, GetSysColorBrush(COLOR_ACTIVEBORDER));
}

void MainWindow::OnCommand(UINT id, UINT notificationCode, HWND hWnd)
{
	switch (id) {
	case ID_FILE_EXIT:
		SendMessage(HWnd(), WM_CLOSE, 0, 0);
	}
}
