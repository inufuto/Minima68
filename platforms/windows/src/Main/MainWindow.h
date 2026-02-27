#pragma once
#include "Direct2DBitmap.h"
#include "../WinApi/ApplicationWindow.h"
#include "../Direct2D/RenderTargetWindow.h"
#include "../Direct2D/RenderTarget.h"
#include "../Direct2D/TextFormat.h"
#include "../Direct2D/Direct2DBrush.h"

class SubWindow : public RenderTargetWindow
{
private:
	TextFormat textFormat;
	SolidColorBrush brush;
	Direct2DBitmap bitmap;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnRender(class RenderTarget& renderTarget) override;
};

class MainWindow : public ApplicationWindow
{
private:
	SubWindow subWindow;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
	virtual void OnPaint(DeviceContext& dc) override;
public:
	MainWindow();
};
