#pragma once
#include "../Direct2D/RenderTargetWindow.h"

class StatusWindow : public RenderTargetWindow
{
	SolidColorBrush backgroundBrush;
	SolidColorBrush textBrush;
	SolidColorBrush highlightBrush;
	SolidColorBrush highlightTextBrush;
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
public:
	auto BackgroundBrush() const { return backgroundBrush.Ptr(); }
	auto TextBrush() const { return textBrush.Ptr(); }
	auto HighlightBrush() const { return highlightBrush.Ptr(); }
	auto HighlightTextBrush() const { return highlightTextBrush.Ptr(); }
};
