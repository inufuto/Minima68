#include "StatusWindow.h"

void StatusWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	RenderTargetWindow::OnCreate(pCreateStruct);
	backgroundBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_WINDOW)));
	textBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_WINDOWTEXT)));
	highlightBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_HIGHLIGHT)));
	highlightTextBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_HIGHLIGHTTEXT)));
}
