#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>

#include "DeviceContext.h"
#include "../../../core/Uncopyable.h"

class CommandTarget abstract :public Uncopyable
{
public:
	virtual ~CommandTarget() = default;
	virtual void OnCommand(UINT id, UINT notificationCode, HWND hWnd) {}
};

class Window abstract : public CommandTarget
{
private:
	static std::vector<Window*> pointers;
	HWND hWnd;
protected:
	void Attach(HWND hWnd) { this->hWnd = hWnd; }
	virtual LPCSTR ClassName();
	virtual void Register();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnWmCreate(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate(CREATESTRUCT* pCreateStruct) {}
	LRESULT OnWmCommand(WPARAM wParam, LPARAM lParam);
	using CommandTarget::OnCommand;
	LRESULT OnWmClose(WPARAM wParam, LPARAM lparam);
	virtual void OnClose() {}
	LRESULT OnWmDestroy(WPARAM wParam, LPARAM lparam);
	virtual void OnDestroy() {}
	LRESULT OnWmSize(WPARAM wParam, LPARAM lParam);
	virtual void OnSize(UINT width, UINT height) {}
	LRESULT OnWmPaint(WPARAM wParam, LPARAM lParam);
	virtual void OnPaint(DeviceContext& dc) {}
	LRESULT OnWmEraseBackground(WPARAM wParam, LPARAM lParam);
	virtual void OnEraseBackground(DeviceContext& dc) {}
	LRESULT OnWmSetFocus(WPARAM wParam, LPARAM lParam);
	virtual void OnSetFocus(HWND hOldWnd) {}
	LRESULT OnWmKillFocus(WPARAM wParam, LPARAM lParam);
	virtual void OnKillFocus(HWND hNewWnd) {}
	LRESULT OnWmKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void OnKeyDown(UINT virtualKey, UINT flags) {}
	LRESULT OnWmKeyUp(WPARAM wParam, LPARAM lParam);
	virtual void OnKeyUp(UINT virtualKey, UINT flags) {}
	LRESULT OnWmLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonDown(UINT flags, POINT point) {}
	LRESULT OnWmMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseMove(UINT flags, POINT point) {}
	LRESULT OnWmLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void OnLButtonUp(UINT flags, POINT point) {}
	LRESULT OnWmMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseWheel(UINT flags, short delta, POINT point) {}
	LRESULT OnWmVScroll(WPARAM wParam, LPARAM lParam);
	virtual void OnVScroll(UINT scrollCode, UINT thumbPos, HWND hScrollBar) {}
	LRESULT OnWmInitMenuPopup(WPARAM wParam, LPARAM lParam);
	virtual void OnInitMenuPopup(HMENU hmenu, UINT index){}
	LRESULT OnWmInitDialog(WPARAM wParam, LPARAM lParam);
	virtual void OnInitDialog();

public:
	Window() { pointers.push_back(this); }
	~Window() override;
	HWND HWnd() const { return hWnd; }
	void Create(DWORD style, HWND hParent, HMENU hMenu);
	void Create(HWND hParent, UINT id);
	void Destroy() const;
	void Move(int x, int y, UINT width, UINT height);
	void Invalidate() { InvalidateRect(hWnd, nullptr, TRUE); }
	std::string GetText() const;
	void SetText(LPCSTR text) const { ::SetWindowText(hWnd, text); }
	void GetClientRect(RECT* pRect) const { ::GetClientRect(hWnd, pRect); }
	HWND GetParent() const { return ::GetParent(hWnd); }
	void SetFocus() const { ::SetFocus(hWnd); }
	BOOL Enable(BOOL enable) const { return ::EnableWindow(hWnd, enable); }
};
