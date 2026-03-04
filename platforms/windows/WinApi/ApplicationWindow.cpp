#include "ApplicationWindow.h"

#include <string>

#include "ApiException.h"
#include "registry.h"

extern LPCSTR CompanyName, ProductName;
auto ApplicationWindow::LayoutRegistry()
{
	char s[256];
	snprintf(s, sizeof(s), "Software\\%s\\%s\\", CompanyName, ProductName);
	return std::string(s);
}

ApplicationWindow* ApplicationWindow::pInstance = nullptr;

LRESULT ApplicationWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CLOSE:
		return OnWmClose(wParam, lParam);
	case WM_DESTROY:
		return Window::OnWmDestroy(wParam, lParam);
	}
	return Window::OnMessage(message, wParam, lParam);
}

void ApplicationWindow::OnClose()
{
	SaveLayout();
	Destroy();
}

int ApplicationWindow::Run(int nCmdShow)
{
	try {
		Create();
		Show(nCmdShow);
		RestoreLayout();
	}
	catch (const ApiException& e) {
		MessageBox(nullptr, e.what(), ProductName, MB_ICONERROR);
		return -1;
	}
	while (true) {
		try {
			MSG msg;
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						return static_cast<int>(msg.wParam);
					}
					if (hAccel == nullptr || !TranslateAccelerator(HWnd(), hAccel, &msg)) {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
			}
			else {
				OnIdle();
			}
			//if (!GetMessage(&msg, nullptr, 0, 0)) {
			//	return static_cast<int>(msg.wParam);
			//}
			//if (hAccel == nullptr || !TranslateAccelerator(HWnd(), hAccel, &msg)) {
			//	TranslateMessage(&msg);
			//	DispatchMessage(&msg);
			//}
		}
		catch (ApiException e) {
			MessageBox(HWnd(), e.what(), ProductName, MB_ICONERROR);
		}
	}
}

void ApplicationWindow::Create() {
	Window::Create(WS_OVERLAPPEDWINDOW, nullptr, hMenu);
}

void ApplicationWindow::Show(int nCmdShow) const
{
	ShowWindow(HWnd(), nCmdShow);
}

void ApplicationWindow::OnDestroy()
{
	PostQuitMessage(0);
}


void ApplicationWindow::SaveLayout()
{
	Registry registory(HKEY_CURRENT_USER, LayoutRegistry().c_str(), true);
	SaveLayout(registory);
}

void ApplicationWindow::SaveLayout(const Registry& registry)
{
	WINDOWPLACEMENT placement;
	GetWindowPlacement(HWnd(), &placement);
	registry.SetDwordValue("show", placement.showCmd);
	registry.SetDwordValue("x", placement.rcNormalPosition.left);
	registry.SetDwordValue("y", placement.rcNormalPosition.top);
	registry.SetDwordValue("cx", placement.rcNormalPosition.right - placement.rcNormalPosition.left);
	registry.SetDwordValue("cy", placement.rcNormalPosition.bottom - placement.rcNormalPosition.top);
}

void ApplicationWindow::RestoreLayout()
{
	Registry registory(HKEY_CURRENT_USER, LayoutRegistry().c_str(), false);
	RestoreLayout(registory);
}

void ApplicationWindow::RestoreLayout(const Registry& registry)
{
	WINDOWPLACEMENT placement;
	GetWindowPlacement(HWnd(), &placement);
	placement.showCmd = registry.GetDwordValue("show", placement.showCmd);
	placement.rcNormalPosition.left = registry.GetDwordValue("x", placement.rcNormalPosition.left);
	placement.rcNormalPosition.top = registry.GetDwordValue("y", placement.rcNormalPosition.top);
	placement.rcNormalPosition.right = placement.rcNormalPosition.left + registry.GetDwordValue("cx", placement.rcNormalPosition.right - placement.rcNormalPosition.left);
	placement.rcNormalPosition.bottom = placement.rcNormalPosition.top + registry.GetDwordValue("cy", placement.rcNormalPosition.bottom - placement.rcNormalPosition.top);
	SetWindowPlacement(HWnd(), &placement);
}
