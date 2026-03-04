#include "ListWindow.h"

void ListWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	StatusWindow::OnCreate(pCreateStruct);
	topIndex = 0;
	selectedIndex = 0;
	UpdateScrollBar();
}

void ListWindow::OnSize(UINT width, UINT height)
{
	StatusWindow::OnSize(width, height);
	UpdateScrollBar();
}

void ListWindow::OnRender(class ::RenderTarget& renderTarget)
{
	StatusWindow::OnRender(renderTarget);
	auto size = renderTarget.GetSize();
	D2D1_RECT_F rect;
	rect.left = 0.0f;
	rect.right = size.width;
	rect.top = 0.0f;
	for (auto index = topIndex; index < topIndex + ItemCount() && rect.top < size.height; index++) {
		rect.bottom = rect.top + ItemHeight();
		DrawItem(renderTarget, rect, index, index == selectedIndex);
		rect.top = rect.bottom;
	}
}

void ListWindow::UpdateScrollBar()
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = ItemCount() - 1;

	auto size = RenderTarget()->GetSize();
	auto visibleItems = static_cast<int>(size.height / ItemHeight());
	si.nPage = visibleItems;
	si.nPos = topIndex;

	SetScrollInfo(HWnd(), SB_VERT, &si, TRUE);
}

void ListWindow::TopIndex(const int index)
{
	this->topIndex = index;
	UpdateScrollBar();
}

void ListWindow::SelectedIndex(const int index)
{
	this->selectedIndex = index;
	UpdateScrollBar();
}
