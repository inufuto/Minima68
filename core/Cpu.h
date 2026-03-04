#pragma once

#include "AbtractEmulator.h"
#include "ClockSource.h"
#include "Uncopyable.h"

struct AssemblyElement
{
	int size;
	std::string mnemonic;
	std::string operand;
};

class Cpu : public ClockDestination, Uncopyable, public RegisterHolder
{
public:
	virtual ~Cpu() = default;
	virtual void Reset() = 0;
	virtual uint16_t LastInstructionAddress() const = 0;
	virtual void BuildAssemblyElement(uint16_t address, AssemblyElement* pElement) const = 0;
};