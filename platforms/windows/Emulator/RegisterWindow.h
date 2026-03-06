#pragma once
#include "StatusWindow.h"
#include "../../../core/AbtractEmulator.h"

class RegisterHolder;

class RegisterWindow : public StatusWindow
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
	void OnRender(class ::RenderTarget& renderTarget) override;
public:
	explicit RegisterWindow(RegisterHolder* pRegisterHolder)
		: pRegisterHolder(pRegisterHolder) {
	}
	auto MinWindowWidth() const { return minWindowWidth; }
	auto MinWindowHeight() const { return minWindowHeight; }
};
