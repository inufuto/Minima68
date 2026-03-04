#include "Font.h"

void Font::Create(const LOGFONT& logFont)
{
	hFont = CreateFontIndirect(&logFont);
}

void Font::Create(int height)
{
	LOGFONT logFont;
	ZeroMemory(&logFont, sizeof(logFont));
	logFont.lfHeight = height;
	logFont.lfPitchAndFamily = FF_DONTCARE;
	Create(logFont);
}

void Font::CreateFixed(int size)
{
	HDC hdc = GetDC(nullptr);
	int pixelHeight = -MulDiv(size, GetDeviceCaps(hdc, LOGPIXELSY), 720);
	ReleaseDC(nullptr, hdc);

	LOGFONT logFont;
	ZeroMemory(&logFont, sizeof(logFont));
	logFont.lfHeight = pixelHeight;
	logFont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	Create(logFont);
}
