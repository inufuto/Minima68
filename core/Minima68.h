#pragma once
#include <cassert>

#include "AbtractEmulator.h"
#include "Cpu6800.h"

class Minima68 : AbstractEmulator
{
public:
	uint16_t StartAddress = 0x0100;
public:
	static constexpr double PrimaryClockFrequency = 8'000'000;
private:
	uint8_t ram[0x10000];
	Cpu6800 cpu;
private:
	class Memory : public MemorySpace
	{
	private:
		Minima68* pOwner;
	public:
		explicit Memory(Minima68* pOwner) : MemorySpace("Memory", 0x10000), pOwner(pOwner) {}
	public:
		uint8_t Read(const uint16_t address) const override { return pOwner->ReadMemory(address); };
		void Write(const uint16_t address, const uint8_t value) override { pOwner->WriteMemory(address, value); }
	} memory;
	uint8_t scrollX;
private:
	uint8_t scrollY;
protected:
	uint8_t ReadMemory(uint16_t address) { return ram[address]; }
	void WriteMemory(uint16_t address, uint8_t value) { ram[address] = value; }
protected:
	explicit Minima68(Debugger* pDebugger = nullptr) : cpu(pDebugger, &memory), memory(this) {}
public:
	uint8_t* Ram() { return ram; }
	auto& Cpu() { return cpu; }
	const auto& Cpu() const { return cpu; }
	void Reset();

	uint8_t ScrollX() const { return scrollX; }
	uint8_t ScrollY() const { return scrollY; }

	int RegisterHolderCount() const override { return 1; }
	const RegisterHolder* RegisterHolderAt(int index) const override { return &cpu; }
	int MemorySpaceCount() const override { return 2; }
	const ::MemorySpace* MemorySpaceAt(int index) const override
	{
		assert(index == 0);
		return &memory;
	}
	
	virtual void SetColor(int index, uint8_t r, uint8_t g, uint8_t b) = 0;
};
