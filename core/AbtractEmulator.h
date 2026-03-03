#pragma once
#include <cstdint>
#include <string>
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

class AbstractEmulator : public Uncopyable
{
public:
	virtual int RegisterHolderCount() const { return 1; }
	virtual const RegisterHolder* RegisterHolderAt(int index) const = 0;

	virtual int MemorySpaceCount() const = 0;
	virtual const ::MemorySpace* MemorySpaceAt(int index) const = 0;
};
