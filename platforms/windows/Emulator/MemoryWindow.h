#pragma once
#include "StatusWindow.h"
#include "../../../core/AbtractEmulator.h"

class MemoryWindow : public StatusWindow
{
private:
	static constexpr int BytesPerLine = 16;
	static constexpr uint32_t AddressRange = 0x10000;
	const ::MemorySpace* pMemorySpace;
private:
	float lineHeight;
	float addressWidth;
	float valueWidth;
	float spaceWidth;
	float marginX;
	int minWindowWidth;
	uint32_t topAddress;
	uint32_t selectedAddress;
	bool isDragging;
private:
	void UpdateScrollBar();
	uint32_t AddressFromPoint(POINT point) const;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnRender(class RenderTarget& renderTarget) override;
	void OnKeyDown(UINT virtualKey, UINT flags) override;
	void OnLButtonDown(UINT flags, POINT point) override;
	void OnMouseMove(UINT flags, POINT point) override;
	void OnLButtonUp(UINT flags, POINT point) override;
	void OnMouseWheel(UINT flags, short delta, POINT point) override;
	void OnVScroll(UINT scrollCode, UINT thumbPos, HWND hScrollBar) override;
public:
	explicit MemoryWindow(const MemorySpace* pMemorySpace) : pMemorySpace(pMemorySpace) {}
	MemoryWindow() = default;
	auto MinWindowWidth() const { return minWindowWidth; }
};
