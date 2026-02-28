#include "ApiException.h"

std::string ApiException::ErrorMessage(DWORD errorCode)
{
	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer),
		0,
		nullptr
	);
	std::string message(messageBuffer, size);
	LocalFree(messageBuffer);
	return message;
}