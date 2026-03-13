#pragma once

#include <cstddef>
#include <cstdint>

// Initializes SPI and mounts a FAT volume via Petit FatFs.
bool InitSdCardPetitFatFs();

// Reads up to bufferSize bytes from a file. Returns false on any mount/open/read error.
bool ReadSdCardFile(const char* path, uint8_t* buffer, size_t bufferSize, size_t* readBytes);
