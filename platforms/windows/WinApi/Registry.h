#pragma once
#include "ApiException.h"
#include "Uncopyable.h"

class Registry : public Uncopyable
{
private:
	HKEY hKey;
public:
	explicit Registry(HKEY hKey, LPCSTR name, bool writable);
	virtual ~Registry();
	HKEY HKey() const { return hKey; }
	
	std::string GetStingValue(LPCSTR name) const;
	void SetStringValue(LPCSTR name, LPCSTR value) const;
	DWORD GetDwordValue(LPCSTR name, DWORD defaultValue) const;
	void SetDwordValue(LPCSTR name, DWORD value) const;
};
