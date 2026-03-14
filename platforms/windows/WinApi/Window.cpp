#include "Window.h"

#include <windowsx.h>
#include "ApiException.h"

extern HINSTANCE HInstance;

std::vector<Window*> Window::pointers;

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_NCCREATE: {
		auto pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		auto pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		pWnd->hWnd = hWnd;
		return pWnd->OnMessage(message, wParam, lParam);
	}
	case WM_NCDESTROY: {
		auto pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		auto lResult = pWnd->OnMessage(message, wParam, lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
		pWnd->hWnd = nullptr;
		return lResult;
	}
	}
	auto pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (pWnd != nullptr) {
		return pWnd->OnMessage(message, wParam, lParam);
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
		return OnWmCommand(wParam, lParam);
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::OnWmCreate(WPARAM wParam, LPARAM lParam)
{
	OnCreate(reinterpret_cast<CREATESTRUCT*>(lParam));
	return 0;
}

LRESULT Window::OnWmDestroy(WPARAM wParam, LPARAM lparam)
{
	OnDestroy();
	return 0;
}

LRESULT Window::OnWmSize(WPARAM wParam, LPARAM lParam)
{
	OnSize(LOWORD(lParam), HIWORD(lParam));
	return 0;
}

LRESULT Window::OnWmPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	DeviceContext dc(ps.hdc);
	OnPaint(dc);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT Window::OnWmEraseBackground(WPARAM wParam, LPARAM lParam)
{
	OnEraseBackground(DeviceContext(reinterpret_cast<HDC>(wParam)));
	return 1;
}

LRESULT Window::OnWmSetFocus(WPARAM wParam, LPARAM lParam)
{
	OnSetFocus(reinterpret_cast<HWND>(wParam));
	return 0;
}

LRESULT Window::OnWmKillFocus(WPARAM wParam, LPARAM lParam)
{
	OnKillFocus(reinterpret_cast<HWND>(wParam));
	return 0;
}

LRESULT Window::OnWmKeyDown(WPARAM wParam, LPARAM lParam)
{
	OnKeyDown(static_cast<UINT>(wParam), static_cast<UINT>(lParam));
	return 0;
}

LRESULT Window::OnWmKeyUp(WPARAM wParam, LPARAM lParam)
{
	OnKeyUp(static_cast<UINT>(wParam), static_cast<UINT>(lParam));
	return 0;
}

LRESULT Window::OnWmChar(WPARAM wParam, LPARAM lParam)
{
	OnChar(static_cast<UINT>(wParam), static_cast<UINT>(lParam));
	return 0;
}

LRESULT Window::OnWmLButtonDown(WPARAM wParam, LPARAM lParam)
{
	OnLButtonDown(static_cast<UINT>(wParam), { (GET_X_LPARAM(lParam)), (GET_Y_LPARAM(lParam)) });
	return 0;
}

LRESULT Window::OnWmMouseMove(WPARAM wParam, LPARAM lParam)
{
	OnMouseMove(static_cast<UINT>(wParam), { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
	return 0;
}

LRESULT Window::OnWmLButtonUp(WPARAM wParam, LPARAM lParam)
{
	OnLButtonUp(static_cast<UINT>(wParam), { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
	return 0;
}

LRESULT Window::OnWmMouseWheel(WPARAM wParam, LPARAM lParam)
{
	OnMouseWheel(static_cast<UINT>(wParam), static_cast<short>(HIWORD(wParam)), { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
	return 0;
}

LRESULT Window::OnWmVScroll(WPARAM wParam, LPARAM lParam)
{
	OnVScroll(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
	return 0;
}

LRESULT Window::OnWmInitMenuPopup(WPARAM wParam, LPARAM lParam)
{
	OnInitMenuPopup(reinterpret_cast<HMENU>(wParam), LOWORD(lParam));
	return 0;
}

LRESULT Window::OnWmInitDialog(WPARAM wParam, LPARAM lParam)
{
	OnInitDialog();
	return TRUE;
}

void Window::OnInitDialog()
{
	auto hParent = GetParent();
	if (hParent == nullptr) {
		hParent = ::GetDesktopWindow();
	}
	RECT parentRect, thisRest;
	GetWindowRect(hParent, &parentRect);
	GetWindowRect(hWnd, &thisRest);
	auto x = parentRect.left + (parentRect.right - parentRect.left - (thisRest.right - thisRest.left)) / 2;
	auto y = parentRect.top + (parentRect.bottom - parentRect.top - (thisRest.bottom - thisRest.top)) / 2;
	Move(x, y, thisRest.right - thisRest.left, thisRest.bottom - thisRest.top);
}

Window::~Window() {
	if (hWnd != nullptr) {
		Destroy();
	}
	pointers.erase(std::remove(pointers.begin(), pointers.end(), this), pointers.end());
}

LRESULT Window::OnWmCommand(WPARAM wParam, LPARAM lParam)
{
	OnCommand(LOWORD(wParam), HIWORD(wParam), reinterpret_cast<HWND>(lParam));
	return 0;
}

LRESULT Window::OnWmClose(WPARAM wParam, LPARAM lparam)
{
	OnClose();
	return 0;
}

LPCSTR Window::ClassName()
{
	return "Inu.Window";
}

void Window::Register()
{
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = &WndProc;
	wc.hInstance = HInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wc.lpszClassName = ClassName();
	ATOM atom = ::RegisterClass(&wc);
	//if (atom == 0) {
	//	throw ApiException();
	//}
}

void Window::Create(DWORD style, HWND hParent, HMENU hMenu)
{
	Register();
	hWnd = ::CreateWindow(
		ClassName(),
		nullptr,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hParent,
		hMenu,
		HInstance,
		this
	);
	if (hWnd == nullptr) {
		throw ApiException();
	}
}

void Window::Create(HWND hParent, UINT id)
{
	Create(WS_CHILD | WS_VISIBLE, hParent, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(id)));
}

void Window::Destroy() const
{
	DestroyWindow(hWnd);
}

void Window::Move(int x, int y, UINT width, UINT height)
{
	MoveWindow(hWnd, x, y, width, height, TRUE);
}

std::string Window::GetText() const
{
	auto length = GetWindowTextLength(hWnd);
	std::string text(length, '\0');
	GetWindowText(hWnd, &text[0], length + 1);
	return text;
}
