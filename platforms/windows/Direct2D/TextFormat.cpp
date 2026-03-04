#include "TextFormat.h"

std::wstring ToWideString(const std::string& narrow)
{
	auto size = MultiByteToWideChar(CP_UTF8, 0, narrow.c_str(), -1, nullptr, 0);
	std::wstring wide(size, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, narrow.c_str(), -1, &wide[0], size);
	return wide;
}

ComPtr<IDWriteFactory> TextFormat::pFactory;

IDWriteFactory* TextFormat::Factory()
{
	if (pFactory == nullptr) {
		IDWriteFactory* p;
		HRESULT result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&p));
		if (FAILED(result)) {
			throw ApiException(result);
		}
		TextFormat::pFactory.Attach(p);
	}
	return pFactory;
}

void TextFormat::Create(LPCSTR fontFamilyName, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch) {
	auto wideString = ToWideString(fontFamilyName);
	HRESULT result = Factory()->CreateTextFormat(wideString.c_str(), nullptr, fontWeight, fontStyle, fontStretch, fontSize, L"", &pTextFormat);
	if (FAILED(result)) {
		throw ApiException(result);
	}
}

void TextLayout::Create(IDWriteTextFormat* pTextFormat, LPCSTR string, FLOAT maxWidth, FLOAT maxHeight) {
	auto wideString = ToWideString(string);
	HRESULT result = TextFormat::Factory()->CreateTextLayout(wideString.c_str(), static_cast<UINT32>(wideString.size()), pTextFormat, maxWidth, maxHeight, &pTextLayout);
	if (FAILED(result)) {
		throw ApiException(result);
	}
}
