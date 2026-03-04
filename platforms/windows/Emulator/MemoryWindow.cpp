#include "MemoryWindow.h"

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
	for (auto leftAddress = topAddress; leftAddress < topAddress + AddressRange && rect.top < size.height; leftAddress += BytesPerLine) {
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
				if (address == selectedAddress)
				{
					pTextBrush = HighlightTextBrush();
					pBackgroundBrush = HighlightBrush();
				}
				else
				{
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
