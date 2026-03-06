#pragma once

#include "AbtractEmulator.h"
#include "ClockSource.h"
#include "Uncopyable.h"

inline uint16_t MakeWord(uint8_t high, uint8_t low) { return high << 8 | low; }
inline uint8_t HighByte(uint16_t word) { return word >> 8; }
inline uint8_t LowByte(uint16_t word) { return word & 0xff; }

struct AssemblyElement
{
	int size;
	std::string mnemonic;
	std::string operand;
};

class Cpu : public ClockDestination, Uncopyable, public RegisterHolder
{
public:
public:
	virtual ~Cpu() = default;
	virtual void Reset() = 0;
	virtual uint16_t LastInstructionAddress() const = 0;
	virtual void BuildAssemblyElement(uint16_t address, AssemblyElement* pElement) const = 0;
};