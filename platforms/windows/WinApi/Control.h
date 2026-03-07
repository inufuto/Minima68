#pragma once
#include "Window.h"

class Control : public Window
{
private:
	WNDPROC wndProc;
protected:
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
public:
	void Subclass(HWND hWnd);
};

class Edit : public Control
{
protected:
	LPCSTR ClassName() override { return "EDIT"; }
};

class Button : public Control
{
protected:
	LPCSTR ClassName() override { return "BUTTON"; }
};