#include <windows.h>
#include "ScreenWindow.h"
#include "Direct2DBrush.h"
#include "EmulatorWindow.h"
#include "resource.h"
#include "../../core/Video.h"

constexpr auto ScreenWidth = XResolution * 2;
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
	auto pFieldRow = pTileMap + emulator.ScrollY() * VramWidth;
	auto fieldYMod = emulator.ScrollY() % TileHeight;
	auto pStatusRow = pTileMap + FieldAreaWidth;
	auto statusYMod = 0;
	for (auto y = 0; y < YResolution; y++) {
		{
			// Field area
			auto pTile = pFieldRow + emulator.ScrollX() / TileWidth;
			auto xMod = emulator.ScrollX() % TileWidth;
			auto tile = *pTile++;
			auto pPattern = pTilePattern +
				(static_cast<uint16_t>(tile) * TilePatternSize) +
				(fieldYMod * TileWidthInBytes) +
				xMod / DotsPerByte;
			auto patternByte = *pPattern++;
			for (auto x = 0; x < FieldWindowWidth * TileWidth; ++x) {
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
						(fieldYMod * TileWidthInBytes);
				}
				if ((xMod % DotsPerByte) == 0) {
					patternByte = *pPattern++;
				}
			}
			if (++fieldYMod >= TileHeight) {
				pFieldRow += VramWidth;
				fieldYMod = 0;
			}
		}
		{
			// Status area
			auto pTile = pStatusRow;
			for (auto tileX = 0; tileX < StatusAreaWidth; ++tileX) {
				auto tile = *pTile++;
				auto pPattern = pTilePattern +
					(static_cast<uint16_t>(tile) * TilePatternSize) +
					(statusYMod * TileWidthInBytes);
				for (auto i = 0; i < TileWidthInBytes; ++i) {
					auto patternByte = *pPattern++;
					*pScreen++ = ColorAt(patternByte >> 4);
					*pScreen++ = ColorAt(patternByte & 0x0f);
				}
			}
			if (++statusYMod >= TileHeight) {
				pStatusRow += VramWidth;
				statusYMod = 0;
			}
		}
	}
	screenBitmap->CopyFromMemory(nullptr, screen, XResolution * sizeof(uint32_t));
	Invalidate();
}
