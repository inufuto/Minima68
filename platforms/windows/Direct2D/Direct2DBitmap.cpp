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

void Direct2DBitmap::Create(RenderTarget& renderTarget, UINT width, UINT height)
{
	auto bitmapProperties = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE));
	HRESULT result = renderTarget->CreateBitmap(D2D1::SizeU(width, height), nullptr, 0, &bitmapProperties, &pBitmap);
	if (FAILED(result)) {
		throw ApiException(result);
	}
}
