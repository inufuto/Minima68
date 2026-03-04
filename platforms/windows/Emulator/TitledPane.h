#pragma once

#include "../WinApi/Window.h"

class TitledPane : public Window
{
private:
	Window* pChild;
private:
	bool active;
	Font font;
private:
	int titleHeight;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnEraseBackground(DeviceContext& dc) override;
	void OnPaint(DeviceContext& dc) override;
public:
	explicit TitledPane(Window* pChild) : pChild(pChild) {}
	bool Active() const { return active; }
	void Active(bool active);
};
