#pragma once
#include <d2d1.h>
#include <dwrite.h>

#include "../WinApi/Uncopyable.h"
#include "../WinApi/ComPtr.h"

std::wstring ToWideString(const std::string& narrow);

class TextFormat : public Uncopyable
{
private:
	static ComPtr<IDWriteFactory> pFactory;
	ComPtr<IDWriteTextFormat> pTextFormat;

public:
	IDWriteTextFormat* Ptr() const { return pTextFormat; }

	static IDWriteFactory* Factory();
	void Create(LPCSTR fontFamilyName, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);
};