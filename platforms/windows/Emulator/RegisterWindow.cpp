#include "RegisterWindow.h"
#include "../Direct2D/RenderTarget.h"

#include <algorithm>
#include <cstdio>

void RegisterWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	ListWindow::OnCreate(pCreateStruct);

	auto charSize = CharSize();
	auto charWidth = charSize.width;
	lineHeight = charSize.height;

	marginX = 4.0f;
	auto count = pRegisterHolder->GetRegisterCount();

	maxNameWidth = 0.0f;
	maxValueWidth = 0.0f;
	for (int i = 0; i < count; ++i) {
		auto nameLength = strlen(pRegisterHolder->GetRegisterName(i));
		auto nameWidth = (nameLength + 1) * charWidth;
		maxNameWidth = (std::max)(nameWidth, maxNameWidth);
		auto valueWidth = pRegisterHolder->GetRegisterSize(i) * 2 * charWidth;
		maxValueWidth = (std::max)(valueWidth, maxValueWidth);
	}

	minWindowWidth = DipToPixel(marginX * 2 + maxNameWidth + maxValueWidth);
}

void RegisterWindow::DrawItem(::RenderTarget& renderTarget, D2D_RECT_F& rect, const int index, bool selected)
{
	ID2D1Brush* pBackgroundBrush;
	ID2D1Brush* pBrush;
	if (selected && GetFocus() == HWnd()) {
		pBackgroundBrush = HighlightBrush();
		pBrush = HighlightTextBrush();
	}
	else {
		pBackgroundBrush = BackgroundBrush();
		pBrush = TextBrush();
	}

	renderTarget.FillRectangle(rect, pBackgroundBrush);

	D2D1_RECT_F nameRect = rect;
	nameRect.left += marginX;
	nameRect.right = nameRect.left + maxNameWidth;

	renderTarget.DrawText(pRegisterHolder->GetRegisterName(index), TextFormat(), nameRect, pBrush);

	D2D1_RECT_F valueRect = rect;
	valueRect.left = nameRect.right;
	valueRect.right = valueRect.left + maxValueWidth;

	char value[32];
	sprintf_s(value, sizeof(value), "%0*X", pRegisterHolder->GetRegisterSize(index) * 2, pRegisterHolder->ReadRegister(index));

	renderTarget.DrawText(value, TextFormat(), valueRect, pBrush);
}
