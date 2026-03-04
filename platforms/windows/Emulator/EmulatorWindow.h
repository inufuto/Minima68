#pragma once
#include "../WinApi/ApplicationWindow.h"

class EmulatorWindow : public ApplicationWindow
{
protected:
	using ApplicationWindow::ApplicationWindow;
//
//	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override
//	{
//		return ApplicationWindow::OnMessage(message, wParam, lParam);
//	}
//
//	void OnCreate(CREATESTRUCT* pCreateStruct) override
//	{
//		ApplicationWindow::OnCreate(pCreateStruct);
//	}
//
//	void OnDestroy() override
//	{
//		ApplicationWindow::OnDestroy();
//	}
//
public:
	~EmulatorWindow() override = default;
};
