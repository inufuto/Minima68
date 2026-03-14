#pragma once
#include "registry.h"
#include "Window.h"

class ApplicationWindow abstract : public Window
{
private:
	static ApplicationWindow* pInstance;
	HMENU hMenu;
	HACCEL hAccel;
protected:
	ApplicationWindow(HMENU hMenu, HACCEL hAccel) : hMenu(hMenu), hAccel(hAccel) {}

	virtual void OnIdle() {}
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	using Window::OnWmClose;
	void OnClose() override;
	void OnDestroy() override;
	void OnInitMenuPopup(HMENU hmenu, UINT index) override;
	virtual bool UpdateMenuItem(HMENU hMenu, UINT id) { return false;}

	static auto LayoutRegistry();
	virtual void SaveLayout();
	virtual void SaveLayout(const Registry& registry);
	virtual void RestoreLayout();
	virtual void RestoreLayout(const Registry& registry);
public:
	static ApplicationWindow& Instance() { return *pInstance; }
	ApplicationWindow() { pInstance = this; }
	~ApplicationWindow() override { pInstance = nullptr; }

	int Run(int nCmdShow);
	virtual void Create();
	void Show(int nCmdShow) const;
};
