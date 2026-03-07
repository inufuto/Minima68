#include "BreakpointDialog.h"

void BreakpointDialog::OnChangeAddress()
{
	auto text = addressEdit.GetText();
	if (text.length() >= 1 && text.length() <= 4) {
		bool valid = true;
		for (char c : text) {
			if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
				valid = false;
				break;
			}
		}
		if (valid) {
			address = static_cast<uint16_t>(std::stoi(text, nullptr, 16));
			okButton.Enable(TRUE);
			return;
		}
	}
	okButton.Enable(FALSE);
}

void BreakpointDialog::OnInitDialog()
{
	Dialog::OnInitDialog();
	addressEdit.Subclass(GetDlgItem(HWnd(), IDC_ADDRESS));
	okButton.Subclass(GetDlgItem(HWnd(), IDOK));
	OnChangeAddress();
}

void BreakpointDialog::OnCommand(UINT id, UINT notificationCode, HWND hWnd)
{
	switch (id) {
	case IDC_ADDRESS:
		switch (notificationCode) {
		case EN_UPDATE:
			OnChangeAddress();
			break;
		}
		break;
	}
	Dialog::OnCommand(id, notificationCode, hWnd);
}
