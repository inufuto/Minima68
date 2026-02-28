#pragma once
#include <functional>

#include "ClockSource.h"
#include "PairRegister.h"
#include "Uncopyable.h"
#include "MemorySpace.h"

class Z80Cpu : ClockDestination, Uncopyable
{
private:
	enum Flags {
		Carry = 1 << 0,
		AddSubtract = 1 << 1,
		ParityOverflow = 1 << 2,
		HalfCarry = 1 << 3,
		Sign = 1 << 6,
		Zero = 1 << 7
	};
	struct ExchangeableRegisters
	{
		PairRegister bc, de, hl;
	} exchangeableRegisterses[2];
	PairRegister afs[2];
	PairRegister ix, iy;
	uint16_t sp;
	uint16_t pc;
	uint8_t iffs[2];
	int exchangeableRegisterIndex;
	int afIndex;
	MemorySpace* pMemorySpace;
	MemorySpace* pIoSpace;
	int clockCountToExecute;
	class Instruction
	{
	public:
		int clockCount;
		std::function<void(Z80Cpu&)> execute;
	};
	static const Instruction Instructions[];
	static const Instruction InstructionsAfterCb[];
	static const Instruction InstructionsAfterEd[];
	static const Instruction InstructionsAfterDd[];
	static const Instruction InstructionsAfterDdCb[];
	static const Instruction InstructionsAfterFd[];
	static const Instruction InstructionsAfterFdCb[];

	const Instruction* pNextInstruction;
protected:
	uint8_t FetchMemory() { return pMemorySpace->Read(pc++); }
	void LoadConstWord(PairRegister& pairRegister);
public:
	Z80Cpu(MemorySpace* pMemorySpace, MemorySpace* pIoSpace) : pMemorySpace(pMemorySpace), pIoSpace(pIoSpace) {}
	ExchangeableRegisters& CurrentExchangeableRegisters() { return exchangeableRegisterses[exchangeableRegisterIndex]; }
	void Reset();
	void FetchInstruction();
	void OnClock(uint32_t time) override;
};
