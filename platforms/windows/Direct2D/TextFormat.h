#pragma once
#include <d2d1.h>
#include <dwrite.h>

#include "../../../core/Uncopyable.h"
#include "../WinApi/ComPtr.h"

std::wstring ToWideString(const std::string& narrow);

class TextFormat : public Uncopyable
{
private:
	static ComPtr<IDWriteFactory> pFactory;
	ComPtr<IDWriteTextFormat> pTextFormat;
public:
	IDWriteTextFormat* operator->() const { return pTextFormat; }
	IDWriteTextFormat* Ptr() const { return pTextFormat; }

	static IDWriteFactory* Factory();
	void Create(LPCSTR fontFamilyName, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL);
};

class TextLayout : public Uncopyable
{
private:
	ComPtr<IDWriteTextLayout> pTextLayout;
public:
	IDWriteTextLayout* operator->() const { return pTextLayout; }
	IDWriteTextLayout* Ptr() const { return pTextLayout; }
	void Create(IDWriteTextFormat* pTextFormat, LPCSTR string, FLOAT maxWidth, FLOAT maxHeight);
	void GetMetrics(DWRITE_TEXT_METRICS* pTextMetrics) const { pTextLayout->GetMetrics(pTextMetrics); }
};