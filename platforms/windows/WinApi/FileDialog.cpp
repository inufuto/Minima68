#include  <shlwapi.h>

#include "FileDialog.h"

#pragma comment(lib, "Shlwapi.lib")

#include "ComPtr.h"
#include "registry.h"

extern LPCSTR CompanyName, ProductName;
extern HINSTANCE HInstance;

static std::string ToDirectory(LPCSTR fileName)
{
	if (fileName == nullptr || fileName[0] == 0) {
		return "";
	}
	TCHAR name[_MAX_PATH + 1];
	strcpy_s(name, fileName);
	PathRemoveFileSpec(name);
	return name;
}

inline char* RegistryKeyName() 
{
	static char s[256];
	snprintf(s, sizeof(s), "Software\\%s\\%s\\Setting", CompanyName, ProductName);
	return s;
}

char* FileFilter(LPCSTR fileType, LPCSTR fileDescription) {
	static char filter[MAX_PATH];
	size_t pos = 0;
	pos += snprintf(filter + pos, sizeof(filter) - pos, "%s (*.%s)", fileDescription, fileType) + 1;
	pos += snprintf(filter + pos, sizeof(filter) - pos, "*.%s", fileType) + 1;
	pos += snprintf(filter + pos, sizeof(filter) - pos, "All Files (*.*)") + 1;
	pos += snprintf(filter + pos, sizeof(filter) - pos, "*.*") + 1;
	filter[pos] = '\0';
	return filter;
}

std::string PromptOpenFileName(LPCSTR fileName, LPCSTR registryName, LPCSTR fileType, LPCSTR fileDescription, HWND hParent)
{
	static constexpr char Separator = '\\';
	auto directory = ToDirectory(fileName);
	if (directory.empty()) {
		Registry registry(HKEY_CURRENT_USER, RegistryKeyName(), false);
		directory = registry.GetStingValue(registryName);
		if (directory.empty()) {
			char s[256];
			GetCurrentDirectory(sizeof(s), s);
			directory = s;
		}
	}
	if (directory.back() != Separator) {
		directory += Separator;
	}

	auto filter = FileFilter(fileType, fileDescription);
	char fileBuffer[MAX_PATH] = "";
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hParent;
	ofn.hInstance = HInstance;
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = sizeof(fileBuffer);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = directory.c_str();
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = fileType;
	if (GetOpenFileName(&ofn)) {
		Registry registry(HKEY_CURRENT_USER, RegistryKeyName(), true);
		registry.SetStringValue(registryName, ToDirectory(ofn.lpstrFile).c_str());
		return ofn.lpstrFile;
	}
	return "";
}
