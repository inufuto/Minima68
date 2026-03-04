#pragma once
#include "ListWindow.h"
#include "../Direct2D/TextFormat.h"
#include "../../../core/AbtractEmulator.h"

class RegisterHolder;

class RegisterWindow : public ListWindow
{
private:
	static constexpr float FontSize = 9.0f * 96 / 72;
	RegisterHolder* pRegisterHolder;
	TextFormat textFormat;
	int minWindowWidth;
private:
	float marginX;
	float lineHeight;
	float maxNameWidth, maxValueWidth;
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	int ItemCount() override { return pRegisterHolder->GetRegisterCount(); }
	float ItemHeight() override { return lineHeight; }
	void DrawItem(::RenderTarget& renderTarget, D2D_RECT_F& rect, int index, bool selected) override;
public:
	explicit RegisterWindow(RegisterHolder* pRegisterHolder)
		: pRegisterHolder(pRegisterHolder) {
	}
	auto MinWindowWidth() const { return minWindowWidth; }
};
