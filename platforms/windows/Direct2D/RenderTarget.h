#pragma once

#include <cassert>
#include <d2d1.h>

#include "Direct2DBitmap.h"
#include "Direct2DBrush.h"
#include "TextFormat.h"
#include "../WinApi/ComPtr.h"
#undef DrawText

class RenderTarget : public Uncopyable
{
private:
	static ComPtr<ID2D1Factory> pFactory;
	ComPtr<ID2D1RenderTarget> pRenderTarget;
protected:
	void Attach(ID2D1RenderTarget* pRenderTarget)
	{
		assert(this->pRenderTarget == nullptr);
		this->pRenderTarget.Attach(pRenderTarget);
	}
public:
	static ID2D1Factory* Factory();
	ID2D1RenderTarget* Ptr() const { return pRenderTarget; }
	ID2D1RenderTarget* operator->() const { return pRenderTarget; }

	void DrawText(std::string string, TextFormat& textFormat,const D2D1_RECT_F& rect, Direct2DBrush& brush);
	void DrawBitmap(Direct2DBitmap& bitmap, D2D1_RECT_F& rect, float opacity, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode);
};

class HwndRenderTarget : public RenderTarget
{
private:
	ID2D1HwndRenderTarget* pHwndRenderTarget;
public:
	ID2D1HwndRenderTarget* operator->() const { return pHwndRenderTarget; }
	void Create(HWND hWnd);
};
