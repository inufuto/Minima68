#include "RenderTarget.h"

ComPtr<ID2D1Factory> RenderTarget::pFactory;

ID2D1Factory* RenderTarget::Factory()
{
	if (pFactory == nullptr) {
		ID2D1Factory* pFactory;
		HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		if (FAILED(result)) {
			throw ApiException(result);
		}
		RenderTarget::pFactory.Attach(pFactory);
	}
	return pFactory;
}

void RenderTarget::DrawText(const LPCTSTR string, IDWriteTextFormat* pTextFormat, const D2D1_RECT_F& rect, ID2D1Brush* pBrush) const
{
	auto wideString = ToWideString(string);
	pRenderTarget->DrawTextA(
		wideString.c_str(),
		static_cast<UINT32>(wideString.size()),
		pTextFormat,
		&rect,
		pBrush
	);
}

void RenderTarget::DrawBitmap(const Direct2DBitmap& bitmap, const D2D1_RECT_F& rect, const float opacity, const D2D1_BITMAP_INTERPOLATION_MODE interpolationMode) const
{
	pRenderTarget->DrawBitmap(
		bitmap.Ptr(),
		rect,
		opacity,
		interpolationMode
	);
}

void HwndRenderTarget::Create(HWND hWnd)
{
	auto renderTargetProperties = D2D1::RenderTargetProperties();
	auto hwndRenderTargetProperties = D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(0, 0));
	Factory()->CreateHwndRenderTarget(&renderTargetProperties, &hwndRenderTargetProperties, &pHwndRenderTarget);
	Attach(pHwndRenderTarget);
}
