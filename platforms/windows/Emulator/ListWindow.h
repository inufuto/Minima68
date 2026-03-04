#pragma once
#include "StatusWindow.h"

class ListWindow : public StatusWindow
{
private:
	int topIndex;
private:
	int selectedIndex;
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnRender(class RenderTarget& renderTarget) override;
	virtual int ItemCount() = 0;
	virtual float ItemHeight() = 0;
	virtual void DrawItem(::RenderTarget& renderTarget, D2D_RECT_F& rect, int index, bool selected) = 0;
	void UpdateScrollBar();
public:
	int TopIndex() const { return topIndex; }
	void TopIndex(int index);
	int SelectedIndex() const { return selectedIndex; }
	void SelectedIndex(int index);
};
