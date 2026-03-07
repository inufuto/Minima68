#pragma once
#include <cstdint>
#include <vector>

#include "MemorySpace.h"
#include "Uncopyable.h"

class RegisterHolder
{
public:
	virtual int GetRegisterCount() const = 0;
	virtual const char* GetRegisterName(int index) const = 0;
	virtual int GetRegisterSize(int index) const = 0;
	virtual uint16_t ReadRegister(int index) const = 0;
	virtual void WriteRegister(int index, uint16_t value) = 0;
};

class BreakpointHolder : public Uncopyable
{
private:
	std::vector<uint16_t> breakpoints;
public:
	virtual const std::vector<uint16_t>& Breakpoints() const { return breakpoints; }
	virtual std::vector<uint16_t>& Breakpoints() { return breakpoints; }
	bool ContainsBreakpoint(uint16_t address) const
	{
		for (auto breakpoint : breakpoints) {
			if (breakpoint == address) {
				return true;
			}
		}
		return false;
	}
};

class AbstractEmulator : public Uncopyable
{
public:
	virtual int RegisterHolderCount() const { return 1; }
	virtual const RegisterHolder* RegisterHolderAt(int index) const = 0;

	virtual int MemorySpaceCount() const = 0;
	virtual const ::MemorySpace* MemorySpaceAt(int index) const = 0;
};
