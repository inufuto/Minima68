#pragma once
#include "ListWindow.h"
#include "../../../core/AbtractEmulator.h"

class RegisterHolder;

class RegisterWindow : public ListWindow
{
private:
	RegisterHolder* pRegisterHolder;
	float marginX;
	float lineHeight;
	float maxNameWidth, maxValueWidth;
	int minWindowWidth;
	int minWindowHeight;
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
	auto MinWindowHeight() const { return minWindowHeight; }
};
