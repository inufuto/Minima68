#include "MemoryWindow.h"

#include <algorithm>
#undef min
#undef max

void MemoryWindow::UpdateScrollBar()
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	si.nMin = 0;
	si.nMax = (AddressRange / BytesPerLine) - 1;

	auto size = RenderTarget()->GetSize();
	auto visibleItems = static_cast<int>(size.height / lineHeight);
	si.nPage = visibleItems;
	si.nPos = topAddress / BytesPerLine;

	SetScrollInfo(HWnd(), SB_VERT, &si, TRUE);
}

LRESULT MemoryWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_KEYDOWN:
		return OnWmKeyDown(wParam, lParam);
	case WM_LBUTTONDOWN:
		return OnWmLButtonDown(wParam, lParam);
	case WM_LBUTTONUP:
		return OnWmLButtonUp(wParam, lParam);
	case WM_MOUSEMOVE:
		return OnWmMouseMove(wParam, lParam);
	case WM_MOUSEWHEEL:
		return OnWmMouseWheel(wParam, lParam);
	case WM_VSCROLL:
		return OnWmVScroll(wParam, lParam);
	}
	return StatusWindow::OnMessage(message, wParam, lParam);
}

void MemoryWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	StatusWindow::OnCreate(pCreateStruct);
	auto charSize = CharSize();
	lineHeight = charSize.height;
	marginX = 4.0f;
	addressWidth = 4 * charSize.width;
	valueWidth = 2 * charSize.width;
	spaceWidth = charSize.width;
	topAddress = selectedAddress = 0;
	isDragging = false;

	minWindowWidth = DipToPixel(marginX * 2 + addressWidth + spaceWidth + (valueWidth + spaceWidth) * BytesPerLine);
}

void MemoryWindow::OnSize(UINT width, UINT height)
{
	StatusWindow::OnSize(width, height);
	UpdateScrollBar();
}

void MemoryWindow::OnRender(class ::RenderTarget& renderTarget)
{
	StatusWindow::OnRender(renderTarget);
	auto size = renderTarget.GetSize();
	D2D1_RECT_F rect;
	rect.top = 0.0f;
	for (auto leftAddress = topAddress; leftAddress <  + AddressRange && rect.top < size.height; leftAddress += BytesPerLine) {
		rect.bottom = rect.top + lineHeight;
		rect.left = 0.0f;
		rect.right = addressWidth;
		char addressText[4 + 1];
		sprintf_s(addressText, sizeof(addressText), "%04X", leftAddress);
		renderTarget.DrawText(addressText, TextFormat(), rect, TextBrush());

		rect.left = rect.right + spaceWidth;
		for (int i = 0; i < BytesPerLine; ++i) {
			rect.right = rect.left + valueWidth;
			uint32_t address = leftAddress + i;
			if (address < AddressRange && rect.left < size.width) {
				uint8_t value = pMemorySpace->Read(address);
				char valueText[2 + 1];
				sprintf_s(valueText, sizeof(valueText), "%02X", value);
				ID2D1Brush* pTextBrush;
				ID2D1Brush* pBackgroundBrush;
				if (address == selectedAddress && GetFocus() == HWnd()) {
					pTextBrush = HighlightTextBrush();
					pBackgroundBrush = HighlightBrush();
				}
				else {
					pTextBrush = TextBrush();
					pBackgroundBrush = BackgroundBrush();
				}
				renderTarget.FillRectangle(rect, pBackgroundBrush);
				renderTarget.DrawText(valueText, TextFormat(), rect, pTextBrush);
			}
			rect.left = rect.right + spaceWidth;
		}
		rect.top = rect.bottom;
	}
}

void MemoryWindow::OnKeyDown(UINT virtualKey, UINT flags)
{
	uint32_t oldAddress = selectedAddress;
	auto size = RenderTarget()->GetSize();
	auto visibleLines = static_cast<int>(size.height / lineHeight);
	auto visibleBytes = visibleLines * BytesPerLine;

	switch (virtualKey) {
	case VK_LEFT:
		if (selectedAddress > 0) {
			selectedAddress--;
		}
		break;
	case VK_RIGHT:
		if (selectedAddress < AddressRange - 1) {
			selectedAddress++;
		}
		break;
	case VK_UP:
		if (selectedAddress >= BytesPerLine) {
			selectedAddress -= BytesPerLine;
		}
		break;
	case VK_DOWN:
		if (selectedAddress + BytesPerLine < AddressRange) {
			selectedAddress += BytesPerLine;
		}
		break;
	case VK_HOME:
		selectedAddress = 0;
		break;
	case VK_END:
		selectedAddress = AddressRange - 1;
		break;
	case VK_PRIOR: // Page Up
		if (selectedAddress >= visibleBytes) {
			selectedAddress -= visibleBytes;
		}
		else {
			selectedAddress = 0;
		}
		break;
	case VK_NEXT: // Page Down
		if (selectedAddress + visibleBytes < AddressRange) {
			selectedAddress += visibleBytes;
		}
		else {
			selectedAddress = AddressRange - 1;
		}
		break;
	default:
		return;
	}

	if (oldAddress != selectedAddress) {
		// Ensure the selected address is visible
		if (selectedAddress < topAddress) {
			topAddress = (selectedAddress / BytesPerLine) * BytesPerLine;
			UpdateScrollBar();
		}
		else if (selectedAddress >= topAddress + visibleBytes) {
			topAddress = ((selectedAddress - visibleBytes + BytesPerLine) / BytesPerLine) * BytesPerLine;
			UpdateScrollBar();
		}

		Invalidate();
	}
}

uint32_t MemoryWindow::AddressFromPoint(POINT point) const
{
	auto dipY = PixelToDip(point.y);
	auto line = static_cast<int>(dipY / lineHeight);
	auto leftAddress = topAddress + line * BytesPerLine;

	auto dipX = PixelToDip(point.x);
	auto valueStartX = marginX + addressWidth + spaceWidth;
	if (dipX >= valueStartX) {
		auto relativeX = dipX - valueStartX;
		auto column = static_cast<int>(relativeX / (valueWidth + spaceWidth));
		if (column >= 0 && column < BytesPerLine) {
			auto address = leftAddress + column;
			if (address < AddressRange) {
				return address;
			}
		}
	}
	return selectedAddress;
}

void MemoryWindow::OnLButtonDown(UINT flags, POINT point)
{
	SetFocus(HWnd());
	isDragging = true;
	SetCapture(HWnd());
	selectedAddress = AddressFromPoint(point);
	Invalidate();
}

void MemoryWindow::OnMouseMove(UINT flags, POINT point)
{
	if (isDragging) {
		selectedAddress = AddressFromPoint(point);
		Invalidate();
	}
}

void MemoryWindow::OnLButtonUp(UINT flags, POINT point)
{
	if (isDragging) {
		isDragging = false;
		ReleaseCapture();
		selectedAddress = AddressFromPoint(point);
		Invalidate();
	}
}

void MemoryWindow::OnMouseWheel(UINT flags, short delta, POINT point)
{
	auto linesToScroll = delta / WHEEL_DELTA;
	auto bytesToScroll = linesToScroll * BytesPerLine;
	int32_t newTopAddress = static_cast<int32_t>(topAddress) + bytesToScroll;
	if (newTopAddress >= 0 && newTopAddress < static_cast<int32_t>(AddressRange)) {
		topAddress = static_cast<uint32_t>(newTopAddress);
		UpdateScrollBar();
		Invalidate();
	}
}

void MemoryWindow::OnVScroll(UINT scrollCode, UINT thumbPos, HWND hScrollBar)
{
	int32_t newTopAddress = static_cast<int32_t>(topAddress);
	auto size = RenderTarget()->GetSize();
	auto visibleLines = static_cast<int>(size.height / lineHeight);

	switch (scrollCode) {
	case SB_LINEUP:
		newTopAddress -= BytesPerLine;
		break;
	case SB_LINEDOWN:
		newTopAddress += BytesPerLine;
		break;
	case SB_PAGEUP:
		newTopAddress -= visibleLines * BytesPerLine;
		break;
	case SB_PAGEDOWN:
		newTopAddress += visibleLines * BytesPerLine;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		newTopAddress = thumbPos * BytesPerLine;
		break;
	default:
		return;
	}

	newTopAddress = std::max(newTopAddress, 0);
	auto maxTopAddress = static_cast<int32_t>(AddressRange - visibleLines * BytesPerLine);
	maxTopAddress = std::max(maxTopAddress, 0);
	newTopAddress = std::min(newTopAddress, maxTopAddress);

	if (topAddress != static_cast<uint32_t>(newTopAddress)) {
		topAddress = static_cast<uint32_t>(newTopAddress);
		UpdateScrollBar();
		Invalidate();
	}
}
