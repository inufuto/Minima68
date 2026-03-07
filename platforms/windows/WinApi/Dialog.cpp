#include "Dialog.h"

#include "ApiException.h"

extern HINSTANCE HInstance;

INT_PTR Dialog::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto pDialog = reinterpret_cast<Dialog*>(GetWindowLongPtr(hDlg, GWLP_USERDATA));
	switch (message) {
	case WM_INITDIALOG:
		pDialog = reinterpret_cast<Dialog*>(lParam);
		SetWindowLongPtr(hDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDialog));
		pDialog->Attach(hDlg);
		return pDialog->OnMessage(message, wParam, lParam);
	case WM_COMMAND:
		return pDialog->OnMessage(message, wParam, lParam);
	}
	return FALSE;
}

LRESULT Dialog::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG:
		return OnWmInitDialog(wParam, lParam);
	case WM_COMMAND:
		return OnWmCommand(wParam, lParam);
	}
	return Window::OnMessage(message, wParam, lParam);
}

void Dialog::OnCommand(UINT id, UINT notificationCode, HWND hWnd)
{
	switch (id)
	{
	case IDOK:
		OnOk();
		return;
	case IDCANCEL:
	case IDCLOSE:
		EndModal(id);
		return;
	}
	Window::OnCommand(id, notificationCode, hWnd);
}

INT_PTR Dialog::ShowModal(HWND hParent)
{
	auto result = DialogBoxParam(HInstance, MAKEINTRESOURCE(id), hParent, Dialog::DlgProc, reinterpret_cast<LPARAM>(this));
	if (result == -1) {
		throw ApiException();
	}
	return result;
}

void Dialog::EndModal(int result) const
{
	BOOL done = EndDialog(HWnd(), result);
	if (!done) {
		throw ApiException();
	}
}
