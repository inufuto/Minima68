#include "StatusWindow.h"

void StatusWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	RenderTargetWindow::OnCreate(pCreateStruct);
	textFormat.Create("Consolas", FontSize);
	backgroundBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_WINDOW)));
	textBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_WINDOWTEXT)));
	highlightBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_HIGHLIGHT)));
	highlightTextBrush.Create(RenderTarget(), D2D1::ColorF(::GetSysColor(COLOR_HIGHLIGHTTEXT)));
}

D2D1_SIZE_F StatusWindow::CharSize()
{
	DWRITE_TEXT_METRICS metrics;
	TextLayout textLayout;
	textLayout.Create(TextFormat(), "W", 1000.0f, 1000.0f);
	textLayout.GetMetrics(&metrics);
	return D2D1::SizeF(metrics.width * 1.1f, metrics.height);
}
