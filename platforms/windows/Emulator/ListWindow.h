#pragma once
#include "StatusWindow.h"

class ListWindow : public StatusWindow
{
private:
	int topIndex;
private:
	int selectedIndex;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnRender(class RenderTarget& renderTarget) override;
	void OnLButtonDown(UINT flags, POINT point) override;
	void OnKeyDown(UINT vk, UINT flags) override;
	void OnMouseWheel(UINT flags, short delta, POINT point) override;
	void OnSetFocus(HWND hOldWnd) override;
	void OnKillFocus(HWND hNewWnd) override { Invalidate(); }
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
