#include "AboutDialog.h"
#include <string>

#ifdef VERSION_INFO_AVAILABLE
#include "version.h"
#else
#define APP_VERSION "Unknown"
#endif

void AboutDialog::OnInitDialog()
{
	Dialog::OnInitDialog();
	
	std::string version = "Version: ";
	version += APP_VERSION;
	SetDlgItemTextA(HWnd(), IDC_VERSION, version.c_str());
}
