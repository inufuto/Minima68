#pragma once
#include <windows.h>
#include <string>

std::string PromptOpenFileName(LPCSTR fileName, LPCSTR registryName, LPCSTR fileType, LPCSTR fileDescription, HWND hParent);
