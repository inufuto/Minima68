#pragma once

#include "WinApi/Dialog.h"
#include "resource.h"

class AboutDialog : public Dialog
{
protected:
	void OnInitDialog() override;
public:
	AboutDialog() : Dialog(IDD_ABOUT) {}
};
