#pragma once
#include "AssemblyWindow.h"
#include "Direct2DBitmap.h"
#include "MemoryWindow.h"
#include "Minima85Win.h"
#include "RegisterWindow.h"
#include "TitledPane.h"
#include "EmulatorWindow.h"
#include "../Direct2D/RenderTargetWindow.h"

class SubWindow : public RenderTargetWindow
{
private:
	TextFormat textFormat;
	SolidColorBrush brush;
	Direct2DBitmap bitmap;
	Minima85Win& emulator;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnRender(class RenderTarget& renderTarget) override;
public:
	explicit SubWindow(Minima85Win& emulator) : emulator(emulator) {}
};

class MainWindow : public EmulatorWindow, public MasterClock::Owner
{
private:
	static constexpr UINT WM_UPDATE_EMULATOR = WM_APP + 1;
	bool waitingForUpdate;
	Minima85Win emulator;
	RegisterWindow registerWindow;
	TitledPane registerPane{ &registerWindow };
	AssemblyWindow assemblyWindow;
	TitledPane assemblyPane{ &assemblyWindow };
	MemoryWindow memoryWindow{ emulator.MemorySpaceAt(0) };
	TitledPane memoryPane{ &memoryWindow };
	SubWindow subWindow;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnDestroy() override;
	void OnSize(UINT width, UINT height) override;
	void OnEraseBackground(DeviceContext& dc) override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
public:
	MainWindow();
	void Invalidate() override;
};
