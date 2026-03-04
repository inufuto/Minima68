#pragma once
#include "../Direct2D/RenderTargetWindow.h"
#include "../Direct2D/TextFormat.h"

class StatusWindow : public RenderTargetWindow
{
protected:
	static constexpr float FontSize = 10.0f * 96 / 72;
private:
	TextFormat textFormat;
	SolidColorBrush backgroundBrush;
	SolidColorBrush textBrush;
	SolidColorBrush highlightBrush;
	SolidColorBrush highlightTextBrush;
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	D2D1_SIZE_F CharSize();
public:
	auto TextFormat() const { return textFormat.Ptr(); }
	auto BackgroundBrush() const { return backgroundBrush.Ptr(); }
	auto TextBrush() const { return textBrush.Ptr(); }
	auto HighlightBrush() const { return highlightBrush.Ptr(); }
	auto HighlightTextBrush() const { return highlightTextBrush.Ptr(); }
};
