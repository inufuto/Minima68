#pragma once

#include <cstddef>
#include <cstdint>

extern bool InitSdCardPetitFatFs();
extern int ReadTextLine(char* buffer, int bufferSize);