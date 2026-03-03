#pragma once
#include "AbtractEmulator.h"
#include "Cpu8085.h"

class Minima85 : AbstractEmulator
{
public:
	static constexpr double MasterClockFrequency = 10000000;
private:
	Cpu8085 cpu;
private:
	class Memory : public MemorySpace
	{
	private:
		Minima85* pOwner;
	public:
		explicit Memory(Minima85* pOwner) : MemorySpace("Memory",0x10000), pOwner(pOwner) {}
	public:
		uint8_t Read(const uint16_t address) const override { return pOwner->ReadMemory(address); };
		void Write(const uint16_t address, const uint8_t value) override { pOwner->WriteMemory(address, value); }
	} memory;
private:
	class Port : public MemorySpace
	{
	private:
		Minima85* pOwner;
	public:
		explicit Port(Minima85* pOwner) : MemorySpace("I/O", 0x100), pOwner(pOwner) {}
		uint8_t Read(const uint16_t address) const override { return pOwner->ReadPort(address); }
		void Write(const uint16_t address, const uint8_t value) override { pOwner->WritePort(address, value); }
	} port;
protected:
	virtual uint8_t ReadMemory(uint16_t address) = 0;
	virtual void WriteMemory(uint16_t address, uint8_t value) = 0;
	virtual uint8_t ReadPort(uint16_t address) { return 0; }
	virtual void WritePort(uint16_t address, uint8_t value) = 0;
public:
	Minima85() : cpu("CPU", &memory, &port), memory(this), port(this) {}
	const Cpu8085& Cpu() const { return cpu; }
	Cpu8085& Cpu() { return cpu; }

	int RegisterHolderCount() const override { return 1; }
	const RegisterHolder* RegisterHolderAt(int index) const override { return &cpu; }
	int MemorySpaceCount() const override { return 2; }
	const ::MemorySpace* MemorySpaceAt(int index) const override;
};
