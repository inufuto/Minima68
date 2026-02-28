#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <windows.h>

#include "DeviceContext.h"
#include "Uncopyable.h"

class CommandTarget abstract :public Uncopyable
{
public:
	virtual ~CommandTarget() = default;

protected:
	virtual void OnCommand(UINT id, UINT notificationCode, HWND hWnd) {}
};

class Window abstract : public CommandTarget
{
private:
private:
	static std::vector<Window*> pointers;
	HWND hWnd;
protected:
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
public:
	Window() { pointers.push_back(this); }
	~Window() override;
	HWND HWnd() const { return hWnd; }
	void Create(DWORD style, HWND hParent, HMENU hMenu);
	void Create(HWND hParent, UINT id);
	void Destroy() const;
	void Move(int x, int y, UINT width, UINT height);
	void Invalidate();
};
