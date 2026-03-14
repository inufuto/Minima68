#include <windows.h>

#include "MainWindow.h"

LPCSTR CompanyName = "Inufuto";
LPCSTR ProductName = "Minima68";
HINSTANCE HInstance;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	::HInstance = hInstance;
	MainWindow mainWindow;
	return mainWindow.Run(nCmdShow);
}
