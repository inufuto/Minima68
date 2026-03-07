#include <windows.h>

#undef min
#undef max
#include "MainWindow.h"
#include "resource.h"

extern HINSTANCE HInstance;
extern LPCSTR ProductName;

MainWindow::MainWindow() :
	EmulatorWindow(::LoadMenu(HInstance, MAKEINTRESOURCE(IDR_MAIN)),
		LoadAccelerators(HInstance, MAKEINTRESOURCE(IDR_MAIN))),
	emulator(this), registerWindow(&emulator.Cpu()),
	breakpointWindow(&emulator.Cpu()),
	assemblyWindow(&emulator.Cpu(), emulator.MemorySpaceAt(0)),
	screenWindow(emulator)
{
}

void MainWindow::UpdateView()
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
		screenWindow.Invalidate();
		UpdateWindow(HWnd());
		waitingForUpdate = false;
		return 0;
	case WM_SELECT_PANE:
		return OnWmSelectPane(wParam, lParam);
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
	breakpointPane.Create(HWnd(), 3);
	breakpointPane.SetText("Breakpoints");
	memoryPane.Create(HWnd(), 4);
	memoryPane.SetText("Memory");
	screenPane.Create(HWnd(), 5);
	screenPane.SetText("Screen");
	SelectPane(2);
	emulator.Start();
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
		{
			auto paneHeight = breakpointPane.TitleHeight() + breakpointWindow.ItemHeightInPixels() * 5;
			breakpointPane.Move(x, 0, paneWidth, paneHeight);
			memoryPane.Move(x, paneHeight, paneWidth, height - paneHeight);
		}
		//memoryPane.Move(x, 0, paneWidth, height);
		x += paneWidth + cxBorder;
	}
	screenPane.Move(x, 0, width - x, height);
}

void MainWindow::OnEraseBackground(DeviceContext& dc)
{
	RECT rect;
	GetClientRect(&rect);
	dc.FillRect(&rect, GetSysColorBrush(COLOR_ACTIVEBORDER));
}

LRESULT MainWindow::OnWmSelectPane(WPARAM wParam, LPARAM lParam)
{
	OnSelectPane(reinterpret_cast<HWND>(lParam));
	return 0;
}

void MainWindow::SelectPane(int index)
{
	if (index != selectedPaneIndex) {
		if (selectedPaneIndex >= 0 && selectedPaneIndex < std::size(selectablePanes)) {
			selectablePanes[selectedPaneIndex]->Active(false);
		}
		selectedPaneIndex = index;
		if (selectedPaneIndex >= 0 && selectedPaneIndex < std::size(selectablePanes)) {
			auto pSelectedPane = selectablePanes[selectedPaneIndex];
			pSelectedPane->Active(true);
			pSelectedPane->SetFocus();
		}
	}
}

void MainWindow::OnSelectPane(HWND hPane)
{
	for (int i = 0; i < std::size(selectablePanes); i++) {
		if (selectablePanes[i]->HWnd() == hPane) {
			SelectPane(i);
			break;
		}
	}
}

bool MainWindow::UpdateMenuItem(HMENU hMenu, UINT id)
{
	switch (id) {
	case ID_NEXT_PANE:
	case ID_PREVIOUS_PANE:
	case ID_EXIT:
	case ID_RESET:
	case ID_ADD_BREAKPOINT:
		return true;
	case ID_DEBUG_RESUME:
	case ID_DEBUG_STEP:
	case ID_DEBUG_NEXT:
		return emulator.Paused();
	case ID_DEBUG_PAUSE:
		return !emulator.Paused();
	case ID_DELETE:
		return breakpointWindow.CanDeleteSelected();
	}
}

void MainWindow::OnCommand(UINT id, UINT notificationCode, HWND hWnd)
{
	switch (id) {
	case ID_EXIT:
		SendMessage(HWnd(), WM_CLOSE, 0, 0);
		break;
	case ID_NEXT_PANE:
		SelectPane((selectedPaneIndex + 1) % std::size(selectablePanes));
		break;
	case ID_PREVIOUS_PANE:
		SelectPane((selectedPaneIndex - 1 + std::size(selectablePanes)) % std::size(selectablePanes));
		break;
	case ID_DEBUG_RESUME:
		if (emulator.Paused()) {
			emulator.Resume();
			UpdateView();
		}
		break;
	case ID_DEBUG_PAUSE:
		if (!emulator.Paused()) {
			emulator.Pause();
			UpdateView();
		}
		break;
	case ID_RESET:
		emulator.Reset();
		break;
	case ID_DEBUG_STEP:
		if (emulator.Paused()) {
			emulator.ExecuteStep();
			UpdateView();
		}
		break;
	case ID_DEBUG_NEXT:
		if (emulator.Paused()) {
			emulator.ExecuteToNext();
			UpdateView();
		}
		break;
	case ID_ADD_BREAKPOINT:
		if (AddBreakpoint(emulator.Cpu())) {
			breakpointWindow.Invalidate();
		}
	case ID_DELETE:
		breakpointWindow.DeleteSelected();
	default:
		EmulatorWindow::OnCommand(id, notificationCode, hWnd);
	}
}
