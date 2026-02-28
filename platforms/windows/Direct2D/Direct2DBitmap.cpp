#include "Direct2DBitmap.h"
#include "RenderTarget.h"

void Direct2DBitmap::Create(class RenderTarget& renderTarget, WicBitmap& wicBitmap)
{
	auto bitmapProperties = D2D1::BitmapProperties();
	HRESULT result = renderTarget->CreateBitmapFromWicBitmap(wicBitmap.Ptr(), &bitmapProperties, &pBitmap);
	if (FAILED(result)) {
		throw ApiException(result);
	}
}

void Direct2DBitmap::CreatePng(class RenderTarget& renderTarget, UINT resourceId)
{
	WicBitmap wicBitmap;
	wicBitmap.CreatePng(resourceId);
	Create(renderTarget, wicBitmap);
}
