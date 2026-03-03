#pragma once
#include "Direct2DBitmap.h"
#include "Minima85Win.h"
#include "../WinApi/ApplicationWindow.h"
#include "../Direct2D/RenderTargetWindow.h"
#include "Emulator/MasterClock.h"

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

class MainWindow : public ApplicationWindow
{
private:
	SubWindow subWindow;
	Minima85Win emulator;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnDestroy() override;
	void OnSize(UINT width, UINT height) override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
	void OnPaint(DeviceContext& dc) override;
public:
	MainWindow();
};
