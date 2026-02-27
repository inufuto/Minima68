#pragma once
#include <stdexcept>

#include "Window.h"

class ApiException : public std::runtime_error
{
	static std::string ErrorMessage(DWORD errorCode);
public:
	explicit ApiException(DWORD errorCode) : std::runtime_error(ErrorMessage(errorCode)) {}
	ApiException() : std::runtime_error(ErrorMessage(GetLastError())) {}
};
