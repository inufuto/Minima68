#pragma once
#include "I8085.h"
#include "Uncopyable.h"

class EmulatorCore : public Uncopyable
{
private:
	I8085 cpu;
	static uint8_t bytes[0x10000];
private:
	class Ram : public MemorySpace
	{
	private:
		EmulatorCore* pOwner;
	public:
		explicit Ram(EmulatorCore* pOwner) : pOwner(pOwner) {}
	public:
		uint8_t Read(uint16_t address) const override { return pOwner->bytes[address]; }
		void Write(uint16_t address, uint8_t value) override { pOwner->bytes[address] = value; }
	} ram;
private:
	class Port : public MemorySpace
	{
	private:
		EmulatorCore* pOwner;
	public:
		explicit Port(EmulatorCore* pOwner) : pOwner(pOwner) {}
		uint8_t Read(uint16_t address) const override { return 0; }
		void Write(uint16_t address, uint8_t value) override
		{
			pOwner->WritePort(address, value);
		}
	} port;
protected:
	virtual void WritePort(uint16_t address, uint8_t value) = 0;
public:
	EmulatorCore() : cpu(&ram, &port), ram(this), port(this) {}
	const I8085& Cpu() const { return cpu; }
	I8085& Cpu() { return cpu; }
	static uint8_t* Bytes() { return bytes; }
};
