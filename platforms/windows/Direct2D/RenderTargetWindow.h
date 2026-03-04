#pragma once
#include "RenderTarget.h"

class RenderTargetWindow : public Window
{
private:
	HwndRenderTarget renderTarget;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnEraseBackground(DeviceContext& dc) override {}
	void OnPaint(DeviceContext& dc) override;
	virtual void OnRender(RenderTarget& renderTarget)
	{
		renderTarget->Clear(D2D1::ColorF(D2D1::ColorF(GetSysColor(COLOR_WINDOW), 1.0f)));
	}
	float GetDpiScale() const;
	UINT DipToPixel(float dip) const;
public:
	HwndRenderTarget& RenderTarget() { return renderTarget; }
};
