#pragma once

#include <cstdint>

class MemorySpace abstract
{
public:
	virtual uint8_t Read(uint16_t address) const = 0;
	virtual void Write(uint16_t address, uint8_t value) = 0;
};