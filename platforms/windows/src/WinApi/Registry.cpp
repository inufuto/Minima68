#include "registry.h"

Registry::Registry(HKEY hKey, LPCSTR name, bool writable)
{
	auto result = RegCreateKeyEx(hKey, name, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_READ | (writable ? KEY_WRITE : 0),nullptr, &this->hKey, nullptr);
	if (result != ERROR_SUCCESS) {
		throw ApiException();
	}
}

Registry::~Registry() {
	if (hKey != nullptr) {
		RegCloseKey(hKey);
		hKey = nullptr;
	}
}

std::string Registry::GetStingValue(LPCSTR name) const
{
	DWORD dataType;
	DWORD dataSize = 0;
	auto result = RegQueryValueEx(hKey, name, nullptr, &dataType, nullptr, &dataSize);
	if (result != ERROR_SUCCESS || dataType != REG_SZ) {
		return "";
	}
	std::string value(dataSize, '\0');
	result = RegQueryValueEx(hKey, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&value[0]), &dataSize);
	if (result != ERROR_SUCCESS) {
		throw ApiException();
	}
	return value;
}

void Registry::SetStringValue(LPCSTR name, LPCSTR value) const
{
	DWORD dataSize = static_cast<DWORD>(strlen(value) + 1);
	LONG result = RegSetValueEx(hKey, name, 0, REG_SZ, reinterpret_cast<const BYTE*>(value), dataSize);
	if (result != ERROR_SUCCESS) {
		throw ApiException();
	}
}

DWORD Registry::GetDwordValue(LPCSTR name, DWORD defaultValue) const
{
	DWORD dataType;
	DWORD dataSize = sizeof(DWORD);
	DWORD value;
	auto result = RegQueryValueEx(hKey, name, nullptr, &dataType, reinterpret_cast<LPBYTE>(&value), &dataSize);
	if (result != ERROR_SUCCESS || dataType != REG_DWORD) {
		return defaultValue;
	}
	return value;
}

void Registry::SetDwordValue(LPCSTR name, DWORD value) const
{
	DWORD dwordValue = value;
	auto result = RegSetValueEx(hKey, name, 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwordValue), sizeof(DWORD));
	if (result != ERROR_SUCCESS) {
		throw ApiException();
	}
}
