#include "BreakpointWindow.h"

#include "EmulatorWindow.h"

int BreakpointWindow::ItemCount()
{
	return static_cast<int>(pHolder->Breakpoints().size());
}

void BreakpointWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	ListWindow::OnCreate(pCreateStruct);
	auto charSize = CharSize();
	itemHeight = charSize.height;
}

void BreakpointWindow::OnLButtonDown(UINT flags, POINT point)
{
	ListWindow::OnLButtonDown(flags, point);
	if (GetFocus() != HWnd()) {
		SetFocus();
		SendMessage(GetParent(), WM_SELECT_PANE, 0, reinterpret_cast<LPARAM>(HWnd()));
		Invalidate();
	}
}

void BreakpointWindow::DrawItem(::RenderTarget& renderTarget, D2D_RECT_F& rect, int index, bool selected)
{
	renderTarget.FillRectangle(rect, selected && GetFocus() == HWnd() ? HighlightBrush() : BackgroundBrush());
	auto breakpointAddress = pHolder->Breakpoints()[index];
	char text[4 + 1];
	sprintf_s(text, sizeof(text), "%04X", breakpointAddress);
	auto textFormat = TextFormat();
	auto textBrush = selected && GetFocus() == HWnd() ? HighlightTextBrush() : TextBrush();
	renderTarget.DrawText(text, textFormat, rect, textBrush);
}

bool BreakpointWindow::CanDeleteSelected() const
{
	return GetFocus() == HWnd() && SelectedIndex() >= 0 && SelectedIndex() < static_cast<int>(pHolder->Breakpoints().size());
}

void BreakpointWindow::DeleteSelected()
{
	if (CanDeleteSelected()) {
		auto index = SelectedIndex();
		pHolder->Breakpoints().erase(pHolder->Breakpoints().begin() + index);
		Invalidate();
	}
}
