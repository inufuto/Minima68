#pragma once
#include "AssemblyWindow.h"
#include "BreakpointWindow.h"
#include "MemoryWindow.h"
#include "RegisterWindow.h"
#include "ScreenWindow.h"
#include "TitledPane.h"
#include "EmulatorWindow.h"

class MainWindow : public EmulatorWindow, public MasterClock::Owner
{
private:
	Minima68Win emulator;
	RegisterWindow registerWindow;
	TitledPane registerPane{ &registerWindow };
	BreakpointWindow breakpointWindow;
	TitledPane breakpointPane{ &breakpointWindow };
	AssemblyWindow assemblyWindow;
	TitledPane assemblyPane{ &assemblyWindow };
	MemoryWindow memoryWindow{ emulator.MemorySpaceAt(0) };
	TitledPane memoryPane{ &memoryWindow };
	ScreenWindow screenWindow;
	TitledPane screenPane{ &screenWindow };
	TitledPane* selectablePanes[3]{ &breakpointPane, &memoryPane,&screenPane };
	int selectedPaneIndex;
	bool waitingForUpdate;
private:
	void SelectPane(int index);
	void UpdateView();
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnDestroy() override;
	void OnSize(UINT width, UINT height) override;
	void OnEraseBackground(DeviceContext& dc) override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
	LRESULT OnWmSelectPane(WPARAM wParam, LPARAM lParam);
	void OnSelectPane(HWND hPane);
	bool UpdateMenuItem(HMENU hMenu, UINT id) override;
public:
	MainWindow();
};
