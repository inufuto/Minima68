#include "WicBitmap.h"

ComPtr<IWICImagingFactory> WicBitmap::pFactory;

IWICImagingFactory* WicBitmap::Factory()
{
	if (pFactory == nullptr) {
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		IWICImagingFactory* p;
		HRESULT result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&p));
		if (FAILED(result)) {
			throw ApiException(result);
		}
		pFactory.Attach(p);
	}
	return pFactory;
}

void WicBitmap::CreatePng(UINT resourceId)
{
	auto hResource = FindResource(nullptr, MAKEINTRESOURCE(resourceId), "PNG");
	if (hResource == nullptr) {
		throw ApiException();
	}
	auto hGlobal = LoadResource(nullptr, hResource);
	if (hGlobal == nullptr) {
		throw ApiException();
	}
	auto pData = LockResource(hGlobal);
	auto dataSize = SizeofResource(nullptr, hResource);
	ComPtr<IStream> pStream;
	HRESULT result = CreateStreamOnHGlobal(nullptr, TRUE, &pStream);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	result = pStream->Write(pData, dataSize, nullptr);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	ComPtr<IWICBitmapDecoder> pDecoder;
	result = Factory()->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	ComPtr<IWICBitmapFrameDecode> pFrame;
	result = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	ComPtr<IWICFormatConverter> pConverter;
	result = Factory()->CreateFormatConverter(&pConverter);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	result = pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
	if (FAILED(result)) {
		throw ApiException(result);
	}
	result = Factory()->CreateBitmapFromSource(pConverter, WICBitmapCacheOnLoad, &pBitmap);
	if (FAILED(result)) {
		throw ApiException(result);
	}
}
