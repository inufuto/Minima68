#pragma once
#include "ListWindow.h"
#include "../../../core/Cpu.h"

class BreakpointWindow : public ListWindow
{
private:
	Cpu* pCpu;
private:
	FLOAT itemHeight;
protected:
	int ItemCount() override;
	float ItemHeight() override { return itemHeight; }

	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnLButtonDown(UINT flags, POINT point) override;

	void DrawItem(::RenderTarget& renderTarget, D2D_RECT_F& rect, int index, bool selected) override;
public:
	explicit BreakpointWindow(Cpu* pCpu) : pCpu(pCpu) {}
	auto ItemHeightInPixels() const { return DipToPixel(itemHeight); }
};
