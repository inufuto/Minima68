#pragma once
#include <d2d1.h>
#include "../WinApi/Uncopyable.h"
#include "../WinApi/ComPtr.h"

class Direct2DBrush : public Uncopyable
{
private:
	ComPtr<ID2D1Brush> pBrush;
protected:
	void Attach(ID2D1Brush* pBrush);
public:
	ID2D1Brush* Ptr() const { return pBrush; }
};

class SolidColorBrush : public Direct2DBrush
{
public:
	void Create(ID2D1RenderTarget* pRenderTarget, D2D1_COLOR_F color);
	void Create(class RenderTarget& renderTarget, D2D1_COLOR_F color);
};