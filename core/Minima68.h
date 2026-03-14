#pragma once

#include <cassert>
#include <queue>

#include "AbtractEmulator.h"
#include "Cpu6800.h"

class Minima68 : public AbstractEmulator
{
public:
	static constexpr uint16_t StartAddress = 0x0100;
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
		uint8_t Read(const uint16_t address) const override;;
		void Write(const uint16_t address, const uint8_t value) override;
	} memory;
	std::queue<char> keyCodes;
protected:
	explicit Minima68(Debugger* pDebugger = nullptr) : cpu(pDebugger, &memory), memory(this) {}

	virtual uint8_t ReadMemory(uint16_t address) const { return ram[address & 0xffff]; }
	virtual void WriteMemory(uint16_t address, uint8_t value) { ram[address & 0xffff] = value; }

	virtual void SetColor(int index, uint8_t r, uint8_t g, uint8_t b) = 0;

	virtual void SetToneSample(int index, const uint8_t* pSample) = 0;
	virtual void SetToneFrequency(int index, uint16_t frequency) = 0;
	virtual void SetToneVolume(int index, uint8_t volume) = 0;
	virtual void SetEffectSample(uint8_t* pSamples) = 0;
	virtual void SetEffectRate(uint8_t rate) = 0;
	virtual void SetEffectVolume(uint8_t volume) = 0;
public:
	uint8_t* Ram() { return ram; }
	auto& Cpu() { return cpu; }
	const auto& Cpu() const { return cpu; }
	void Reset();
	void MakeInterrupt() { cpu.MakeInterrupt(); }

	uint8_t ScrollX() const;
	uint8_t ScrollY() const;

	int RegisterHolderCount() const override { return 1; }
	const RegisterHolder* RegisterHolderAt(int index) const override { return &cpu; }
	int MemorySpaceCount() const override { return 2; }
	const ::MemorySpace* MemorySpaceAt(int index) const override
	{
		assert(index == 0);
		return &memory;
	}
	void AddKeyCode(char charCode) { keyCodes.push(charCode); }
	char RetrieveKeyCode();
};
