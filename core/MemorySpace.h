#pragma once

#include <cstdint>
#include <string>

class MemorySpace
{
private:
	std::string name;
	uint32_t size;
public:
	MemorySpace(const char* name, uint32_t size) : name(name), size(size) {}
	virtual ~MemorySpace() = default;
	const char* Name() const { return name.c_str(); }
	uint32_t Size() const { return size; }
	virtual uint8_t Read(uint16_t address) const = 0;
	virtual void Write(uint16_t address, uint8_t value) = 0;
};
