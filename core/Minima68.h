#pragma once
#include <cassert>

#include "AbtractEmulator.h"
#include "Cpu6800.h"

class Minima68 : AbstractEmulator
{
public:
	uint16_t StartAddress = 0x0100;
public:
	static constexpr double MasterClockFrequency = 8'000'000;
private:
	Cpu6800 cpu;
private:
	class Memory : public MemorySpace
	{
	private:
		Minima68* pOwner;
	public:
		explicit Memory(Minima68* pOwner) : MemorySpace("Memory",0x10000), pOwner(pOwner) {}
	public:
		uint8_t Read(const uint16_t address) const override { return pOwner->ReadMemory(address); };
		void Write(const uint16_t address, const uint8_t value) override { pOwner->WriteMemory(address, value); }
	} memory;
protected:
	virtual uint8_t ReadMemory(uint16_t address) = 0;
	virtual void WriteMemory(uint16_t address, uint8_t value) = 0;
protected:
	Minima68() : cpu(&memory), memory(this) {}
	void Reset();
public:
	auto& Cpu() { return cpu; }
	const auto& Cpu() const { return cpu; }
	int RegisterHolderCount() const override { return 1; }
	const RegisterHolder* RegisterHolderAt(int index) const override { return &cpu; }
	int MemorySpaceCount() const override { return 2; }
	const ::MemorySpace* MemorySpaceAt(int index) const override
	{
		assert(index == 0);
		return &memory;
	}
};
