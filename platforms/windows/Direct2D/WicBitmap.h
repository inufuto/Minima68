#pragma once
#include <wincodec.h>
#include "../../../core/Uncopyable.h"
#include "../WinApi/ComPtr.h"

class WicBitmap : public Uncopyable
{
private:
	static ComPtr<IWICImagingFactory> pFactory;
	ComPtr<IWICBitmap> pBitmap;
public:
	class Lock : public Uncopyable
	{
	private:
		ComPtr<IWICBitmapLock> pBitmapLock;
	public:
		Lock(IWICBitmap* pBitmap, DWORD flags);
		IWICBitmapLock* Ptr() const { return pBitmapLock; }
	};
public:
	static IWICImagingFactory* Factory();
	IWICBitmap* Ptr() const { return pBitmap; }
	void CreatePng(UINT resourceId);
};
