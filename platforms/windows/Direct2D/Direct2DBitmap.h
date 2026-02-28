#pragma once
#include <d2d1.h>

#include "WicBitmap.h"

class Direct2DBitmap : public Uncopyable
{
private:
	ComPtr<ID2D1Bitmap> pBitmap;
public:
	ID2D1Bitmap* Ptr() const { return pBitmap; }
	void Create(class RenderTarget& renderTarget, WicBitmap& wicBitmap);
	void CreatePng(class RenderTarget& renderTarget, UINT resourceId);
};
