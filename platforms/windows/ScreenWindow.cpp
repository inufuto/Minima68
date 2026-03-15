#include <windows.h>
#include "ScreenWindow.h"
#include "EmulatorWindow.h"
#include "../../core/Video.h"
#include "../../core/Input.h"

constexpr auto ScreenWidth = XResolution * 2 * 115 / 100;
constexpr auto ScreenHeight = YResolution;

void ScreenPane::OnCreate(CREATESTRUCT* pCreateStruct)
{
	TitledPane::OnCreate(pCreateStruct);
	xMargin = GetSystemMetrics(SM_CXFRAME) * 2;
	yMargin = GetSystemMetrics(SM_CYFRAME) * 2;
}

void ScreenPane::OnSize(UINT width, UINT height)
{
	auto maxHeight = height - TitleHeight() - yMargin * 2;
	auto maxWidth = width - xMargin * 2;
	auto targetWidth = maxWidth;
	auto targetHeight = maxWidth * ScreenHeight / ScreenWidth;
	if (targetHeight > maxHeight) {
		targetHeight = maxHeight;
		targetWidth = maxHeight * ScreenWidth / ScreenHeight;
	}
	auto x = (maxWidth - targetWidth) / 2 + xMargin;
	auto y = TitleHeight() + (maxHeight - targetHeight) / 2 + yMargin;
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
	case WM_KEYDOWN:
		return OnWmKeyDown(wParam, lParam);
	case WM_KEYUP:
		return OnWmKeyUp(wParam, lParam);
	case WM_CHAR:
		return OnWmChar(wParam, lParam);
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

void ScreenWindow::OnKeyDown(UINT virtualKey, UINT flags)
{
	emulator.SetJoystickBit(ToBit(virtualKey));
}

void ScreenWindow::OnKeyUp(UINT virtualKey, UINT flags)
{
	emulator.ClearJoystickBit(ToBit(virtualKey));
}

void ScreenWindow::OnChar(UINT charCode, UINT flags)
{
	emulator.AddKeyCode(charCode);
}

uint8_t ScreenWindow::ToBit(UINT virtualKey)
{
	static const struct {
		UINT virtualKey;
		uint8_t bit;
	} mapping[] = {
		{ VK_UP, Joystick::Up },
		{ VK_DOWN, Joystick::Down },
		{ VK_LEFT, Joystick::Left },
		{ VK_RIGHT, Joystick::Right },
		{ 'Z', Joystick::Button0 },
		{ 'X', Joystick::Button1 },
	};
	for (const auto& m : mapping) {
		if (virtualKey == m.virtualKey) {
			return m.bit;
		}
	}
	return 0;
}

void ScreenWindow::UpdateScreenBitmap()
{
	auto pScreen = screen;
	{
		// Background
		auto pTile = emulator.TileMap();
		for (auto tileY = 0; tileY < VramHeight; tileY++) {
			for (auto tileX = 0; tileX < VramWidth; ++tileX) {
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
			pScreen += XResolution * TileHeight - TileWidth * VramWidth;
		}
	}
	{
		// Sprites
		auto pScreenRow = screen;
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
