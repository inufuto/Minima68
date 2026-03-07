#include "ListWindow.h"

#include "EmulatorWindow.h"

LRESULT ListWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_LBUTTONDOWN:
		return OnWmLButtonDown(wParam, lParam);
	case WM_KEYDOWN:
		return OnWmKeyDown(wParam, lParam);
	case WM_MOUSEWHEEL:
		return OnWmMouseWheel(wParam, lParam);
	case WM_SETFOCUS:
		return OnWmSetFocus(wParam, lParam);
	case WM_KILLFOCUS:
		return OnWmKillFocus(wParam, lParam);
	}
	return StatusWindow::OnMessage(message, wParam, lParam);
}

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
	for (auto index = topIndex; index < ItemCount() && rect.top < size.height; index++) {
		rect.bottom = rect.top + ItemHeight();
		DrawItem(renderTarget, rect, index, index == selectedIndex);
		rect.top = rect.bottom;
	}
}

void ListWindow::OnLButtonDown(UINT flags, POINT point)
{
	StatusWindow::OnLButtonDown(flags, point);

	auto itemHeight = ItemHeight();
	if (itemHeight > 0) {
		float dipY = PixelToDip(point.y);
		int clickedIndex = topIndex + static_cast<int>(dipY / itemHeight);
		if (clickedIndex >= 0 && clickedIndex < ItemCount()) {
			SelectedIndex(clickedIndex);
		}
	}
}

void ListWindow::OnKeyDown(UINT vk, UINT flags)
{
	StatusWindow::OnKeyDown(vk, flags);

	int newSelectedIndex = selectedIndex;
	auto size = RenderTarget()->GetSize();
	auto itemHeight = ItemHeight();
	int visibleItems = itemHeight > 0 ? static_cast<int>(size.height / itemHeight) : 1;

	switch (vk) {
	case VK_UP:
		if (selectedIndex > 0) {
			newSelectedIndex = selectedIndex - 1;
		}
		break;
	case VK_DOWN:
		if (selectedIndex < ItemCount() - 1) {
			newSelectedIndex = selectedIndex + 1;
		}
		break;
	case VK_HOME:
		newSelectedIndex = 0;
		break;
	case VK_END:
		newSelectedIndex = ItemCount() - 1;
		break;
	case VK_PRIOR:
		newSelectedIndex = selectedIndex - visibleItems;
		if (newSelectedIndex < 0) {
			newSelectedIndex = 0;
		}
		break;
	case VK_NEXT:
		newSelectedIndex = selectedIndex + visibleItems;
		if (newSelectedIndex >= ItemCount()) {
			newSelectedIndex = ItemCount() - 1;
		}
		break;
	default:
		return;
	}

	if (newSelectedIndex != selectedIndex) {
		SelectedIndex(newSelectedIndex);

		if (newSelectedIndex < topIndex) {
			TopIndex(newSelectedIndex);
		} else if (newSelectedIndex >= topIndex + visibleItems) {
			TopIndex(newSelectedIndex - visibleItems + 1);
		}

		Invalidate();
	}
}

void ListWindow::OnMouseWheel(UINT flags, short delta, POINT point)
{
	StatusWindow::OnMouseWheel(flags, delta, point);

	int scrollLines = 3;
	int scrollAmount = (delta > 0) ? -scrollLines : scrollLines;

	int newTopIndex = topIndex + scrollAmount;
	if (newTopIndex < 0) {
		newTopIndex = 0;
	}

	auto size = RenderTarget()->GetSize();
	auto itemHeight = ItemHeight();
	int visibleItems = itemHeight > 0 ? static_cast<int>(size.height / itemHeight) : 1;
	int maxTopIndex = ItemCount() - visibleItems;
	if (maxTopIndex < 0) {
		maxTopIndex = 0;
	}
	if (newTopIndex > maxTopIndex) {
		newTopIndex = maxTopIndex;
	}

	if (newTopIndex != topIndex) {
		TopIndex(newTopIndex);
		Invalidate();
	}
}

void ListWindow::OnSetFocus(HWND hOldWnd)
{
	SendMessage(GetParent(), WM_SELECT_PANE, 0, reinterpret_cast<LPARAM>(HWnd()));
	Invalidate();
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
	if (this->selectedIndex != index)
	{
		this->selectedIndex = index;
		Invalidate();
	}
	UpdateScrollBar();
}
