#pragma once
#include "Window.h"

class Dialog : public Window
{
private:
	UINT id;
private:
	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnOk() { EndModal(IDOK); }
	LRESULT OnMessage(UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnCommand(UINT id, UINT notificationCode, HWND hWnd) override;
public:
	explicit Dialog(UINT id) : id(id) {}
	INT_PTR ShowModal(HWND hParent = nullptr);
	void EndModal(int result) const;
};
