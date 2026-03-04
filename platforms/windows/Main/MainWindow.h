#pragma once
#include "Direct2DBitmap.h"
#include "Minima85Win.h"
#include "RegisterWindow.h"
#include "../WinApi/ApplicationWindow.h"
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

class MainWindow : public ApplicationWindow, public MasterClock::Owner
{
private:
	Minima85Win emulator;
	RegisterWindow registerWindow;
	SubWindow subWindow;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnDestroy() override;
	void OnSize(UINT width, UINT height) override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
	void OnPaint(DeviceContext& dc) override;
public:
	MainWindow();
	void Invalidate() override;
};
