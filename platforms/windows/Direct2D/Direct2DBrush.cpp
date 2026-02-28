#include "Direct2DBrush.h"
#include "RenderTarget.h"

void Direct2DBrush::Attach(ID2D1Brush* pBrush) {
	assert(this->pBrush == nullptr);
	this->pBrush.Attach(pBrush);
}

void SolidColorBrush::Create(ID2D1RenderTarget* pRenderTarget, D2D1_COLOR_F color) {
	ID2D1SolidColorBrush* pSolidColorBrush;
	HRESULT result = pRenderTarget->CreateSolidColorBrush(color, &pSolidColorBrush);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	Attach(pSolidColorBrush);
}

void SolidColorBrush::Create(class RenderTarget& renderTarget, D2D1_COLOR_F color)
{
	Create(renderTarget.Ptr(), color);
}
