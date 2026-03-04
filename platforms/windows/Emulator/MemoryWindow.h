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
private:
	void UpdateScrollBar();
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnRender(class RenderTarget& renderTarget) override;
public:
	explicit MemoryWindow(const MemorySpace* pMemorySpace) : pMemorySpace(pMemorySpace) {}
	MemoryWindow() = default;
	auto MinWindowWidth() const { return minWindowWidth; }
};
