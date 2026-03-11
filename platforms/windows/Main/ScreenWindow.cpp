#include <windows.h>
#include "ScreenWindow.h"
#include "EmulatorWindow.h"
#include "../../core/Video.h"

constexpr auto ScreenWidth = XResolution * 2 * 115 / 100;
constexpr auto ScreenHeight = YResolution;

void ScreenPane::OnSize(UINT width, UINT height)
{
	auto maxHeight = height - TitleHeight();
	auto targetWidth = width;
	auto targetHeight = width * ScreenHeight / ScreenWidth;
	if (targetHeight > maxHeight) {
		targetHeight = maxHeight;
		targetWidth = maxHeight * ScreenWidth / ScreenHeight;
	}
	auto x = (width - targetWidth) / 2;
	auto y = TitleHeight() + (maxHeight - targetHeight) / 2;
	Child()->Move(x, y, targetWidth, targetHeight);
}

LRESULT ScreenWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
		return OnWmCreate(wParam, lParam);
	case WM_PAINT:
		return OnWmPaint(wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnWmLButtonDown(wParam, lParam);
	case WM_SETFOCUS:
		return OnWmSetFocus(wParam, lParam);
	}
	return RenderTargetWindow::OnMessage(message, wParam, lParam);
}

void ScreenWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	RenderTargetWindow::OnCreate(pCreateStruct);
	screenBitmap.Create(RenderTarget(), XResolution, YResolution);
}

void ScreenWindow::OnPaint(DeviceContext& dc)
{
	RenderTargetWindow::OnPaint(dc);
	UpdateScreenBitmap();
}

void ScreenWindow::OnRender(::RenderTarget& renderTarget)
{
	RenderTargetWindow::OnRender(renderTarget);
	auto size = renderTarget.GetSize();
	auto rect = D2D1::RectF(0.0f, 0.0f, size.width, size.height);
	renderTarget.DrawBitmap(screenBitmap, rect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

void ScreenWindow::OnSetFocus(HWND hOldWnd)
{
	SendMessage(GetParent(), WM_SELECT_PANE, 0, reinterpret_cast<LPARAM>(HWnd()));
}

void ScreenWindow::UpdateScreenBitmap()
{
	auto pScreen = screen;
	{
		// Status area
		auto pTile = pTileMap;
		for (auto tileY = 0; tileY < StatusAreaHeight; tileY++) {
			for (auto tileX = 0; tileX < WindowWidth; ++tileX) {
				auto tile = *pTile++;
				auto pPattern = pTilePattern + static_cast<uint16_t>(tile) * TilePatternSize;
				for (auto y = 0; y < TileHeight; ++y) {
					for (auto i = 0; i < TileWidthInBytes; ++i) {
						auto patternByte = *pPattern++;
						*pScreen++ = ColorAt(patternByte >> 4);
						*pScreen++ = ColorAt(patternByte & 0x0f);
					}
					pScreen += (XResolution - TileWidth);
				}
				pScreen += TileWidth - XResolution * TileHeight;
			}
			pScreen += XResolution * TileHeight - TileWidth * WindowWidth;
			pTile += VramWidth - WindowWidth;
		}
	}
	{
		// Field area
		uint8_t scrollY = emulator.ScrollY();
		uint8_t scrollX = emulator.ScrollX();
		auto pTileRow = pTileMap + VramWidth * StatusAreaHeight +
			scrollY / TileHeight * VramWidth +
			scrollX / TileWidth;
		auto yMod = scrollY % TileHeight;
		auto xModLeft = scrollX % TileWidth;
		for (auto y = 0; y < YResolution - TileHeight * StatusAreaHeight; y++) {
			auto pTile = pTileRow;
			auto xMod = xModLeft;
			auto tile = *pTile++;
			auto pPattern = pTilePattern +
				(static_cast<uint16_t>(tile) * TilePatternSize) +
				yMod * TileWidthInBytes +
				xMod / DotsPerByte;
			auto patternByte = *pPattern++;
			for (auto x = 0; x < XResolution; ++x) {
				if ((xMod % DotsPerByte) == 0) {
					*pScreen++ = ColorAt(patternByte >> 4);
				}
				else {
					*pScreen++ = ColorAt(patternByte & 0x0f);
				}
				if (++xMod >= TileWidth) {
					xMod = 0;
					tile = *pTile++;
					pPattern = pTilePattern +
						(static_cast<uint16_t>(tile) * TilePatternSize) +
						yMod * TileWidthInBytes;
				}
				if ((xMod % DotsPerByte) == 0) {
					patternByte = *pPattern++;
				}
			}
			if (++yMod >= TileHeight) {
				pTileRow += VramWidth;
				yMod = 0;
			}
		}
	}
	{
		// Sprites
		auto pScreenRow = screen + XResolution * TileHeight * StatusAreaHeight;
		for (auto y = 0; y < SpriteRangeY; ++y) {
			auto horizontalCount = 0;
			auto pSprite = pSpriteAttributes + SpriteCount;
			for (auto i = 0; i < SpriteCount; ++i) {
				--pSprite;
				uint8_t yOffset = y - pSprite->y;
				if (yOffset < SpriteHeight) {
					uint8_t x = pSprite->x;
					auto pPattern = pSpritePattern +
						(static_cast<uint16_t>(pSprite->pattern) * SpritePatternSize);
					pPattern += yOffset * SpriteWidth / DotsPerByte;
					auto pScreenDot = pScreenRow + x;
					for (auto j = 0; j < SpriteWidth / DotsPerByte; ++j) {
						auto patternByte = *pPattern++;
						if (x < XResolution) {
							auto dot = patternByte >> 4;
							if (dot != 0) {
								*pScreenDot = ColorAt(dot);
							}
						}
						++pScreenDot;
						++x;
						if (x < XResolution) {
							auto dot = patternByte & 0x0f;
							if (dot != 0) {
								*pScreenDot = ColorAt(dot);
							}
						}
						++pScreenDot;
						++x;
					}
					if (++horizontalCount >= MaxHorizontalSpriteCount) {
						break;
					}
				}
			}
			pScreenRow += XResolution;
		}
	}
	screenBitmap->CopyFromMemory(nullptr, screen, XResolution * sizeof(uint32_t));
	Invalidate();
}
