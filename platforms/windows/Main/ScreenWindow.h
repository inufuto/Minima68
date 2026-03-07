#pragma once
#include "Direct2DBitmap.h"
#include "Minima68Win.h"
#include "../Direct2D/RenderTargetWindow.h"

class ScreenWindow : public RenderTargetWindow
{
private:
	TextFormat textFormat;
	SolidColorBrush brush;
	Direct2DBitmap bitmap;
	Minima68Win& emulator;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnRender(class RenderTarget& renderTarget) override;
	void OnLButtonDown(UINT flags, POINT point) override { SetFocus(); }
	void OnSetFocus(HWND hOldWnd) override;
public:
	explicit ScreenWindow(Minima68Win& emulator) : emulator(emulator) {}
};
