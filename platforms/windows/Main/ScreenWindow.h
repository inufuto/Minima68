#pragma once
#include "Direct2DBitmap.h"
#include "MemoryMap.h"
#include "Minima68Win.h"
#include "TitledPane.h"
#include "RenderTargetWindow.h"

class ScreenPane : public TitledPane
{
protected:
	void OnSize(UINT width, UINT height) override;
public:
	explicit ScreenPane(Window* pChild) : TitledPane(pChild) {}
};

class ScreenWindow : public RenderTargetWindow
{
private:
	Minima68Win& emulator;
	Direct2DBitmap screenBitmap;
	uint32_t screen[XResolution * YResolution];
	uint8_t* pTileMap = emulator.Ram() + TileMapAddress;
	uint8_t* pTilePattern = emulator.Ram() + TilePatternAddress;
	uint8_t* pSpritePattern = emulator.Ram() + SpritePatternAddress;
	SpriteAttribute* pSpriteAttributes = reinterpret_cast<SpriteAttribute*>(emulator.Ram() + SpriteAttributeAddress);
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnPaint(DeviceContext& dc) override;
	void OnRender(class RenderTarget& renderTarget) override;
	void OnLButtonDown(UINT flags, POINT point) override { SetFocus(); }
	void OnSetFocus(HWND hOldWnd) override;
private:
	uint32_t ColorAt(uint8_t index) const { assert(index < ColorCount); return emulator.ColorAt(index); }
	void UpdateScreenBitmap();
public:
	explicit ScreenWindow(Minima68Win& emulator) : emulator(emulator) {}
};
