#pragma once
#include <functional>

#include "ClockSource.h"
#include "Uncopyable.h"
#include "MemorySpace.h"

inline uint16_t MakeWord(uint8_t high, uint8_t low)
{
	return high << 8 | low;
}
inline uint8_t HighByte(uint16_t word)
{
	return word >> 8;
}
inline uint8_t LowByte(uint16_t word)
{
	return word & 0xff;
}

union PairRegister
{
	struct {
		uint8_t low;
		uint8_t high;
	};
	uint16_t word;
};

class Z80Cpu : ClockDestination, Uncopyable
{
private:
	struct Flag {
		static constexpr uint8_t Carry = 1 << 0;
		static constexpr uint8_t AddSubtract = 1 << 1;
		static constexpr uint8_t ParityOverflow = 1 << 2;
		static constexpr uint8_t HalfCarry = 1 << 3;
		static constexpr uint8_t Sign = 1 << 6;
		static constexpr uint8_t Zero = 1 << 7;
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
	uint8_t interrupt, refresh;
	MemorySpace* pMemorySpace;
	MemorySpace* pIoSpace;
	int clockCountToExecute;
	bool halted;

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
private:
	ExchangeableRegisters& CurrentExchangeableRegisters() { return exchangeableRegisterses[exchangeableRegisterIndex]; }
	PairRegister& CurrentAf() { return afs[afIndex]; }
	uint8_t FetchByte() { return pMemorySpace->Read(pc++); }
	uint16_t FetchWord();
	void PushWord(uint16_t value);
	uint16_t PopWord();
	void FetchInstruction();

	void SetFlag(uint8_t flag) { CurrentAf().low |= flag; }
	void ClearFlag(uint8_t flag) { CurrentAf().low &= ~flag; }
	void UpdateZeroSignParityFlags(uint8_t value);
	
	void LoadConstWord(uint16_t& wordRegister);
	void LoadByteConst(uint8_t& byteRegister);
	void LoadByte(uint8_t& byteRegister, uint16_t address);
	void StoreByteToMemory(uint8_t byteRegister);
	void StoreByteConst(uint16_t address);
	void AddByteConst(uint8_t& byteRegister);
	void AddByteRegister(uint8_t& byteRegister, int offset);
	void AddByteMemory(uint16_t address, int offset);
	void AddByteConstCarry(uint8_t& byteRegister);
	void AddByteRegisterCarry(uint8_t& byteRegister, uint8_t offset);
	void SubtractByteConst(uint8_t& byteRegister);
	void SubtractByteRegister(uint8_t& byteRegister, uint8_t value);
	void SubtractByteMemory(uint16_t address, uint8_t value);
	void SubtractByteRegisterCarry(uint8_t& byteRegister, uint8_t value);
	void AndByteConst(uint8_t& byreRegister);
	void AndByteRegister(uint8_t& byteRegister, uint8_t value);
	void XorByteConst(uint8_t& byteRegister);
	void XorByteRegister(uint8_t& byteRegister, uint8_t value);
	void OrByteConst(uint8_t& byteRegister);
	void OrByteRegister(uint8_t& byteRegister, uint8_t value);
	void CompareByteConst(uint8_t byteRegister);
	void CompareByteRegister(uint8_t byteRegister, uint8_t value);
	void Complement(uint8_t& byteRegister);
	void Negate(uint8_t& byteRegiter);
	void RotateLeftCircular(uint8_t& byteRegister);
	void RotateLeftCircularMemory(uint16_t address);
	void RotateLeft(uint8_t& byteRegister);
	void RotateLeftMemory(uint16_t address);
	void RotateRightCircular(uint8_t& byteRegister);
	void RotateRightCircularMemory(uint16_t address);
	void RotateRight(uint8_t& byteRegister);
	void RotateRightMemory(uint16_t address);
	void ShiftLeftArithmetic(uint8_t& byteRegister);
	void ShiftLeftArithmeticMemory(uint16_t address);
	void ShiftRightArithmetic(uint8_t& byteRegister);
	void ShiftRightArithmeticMemory(uint16_t address);
	void ShiftLeftLogical(uint8_t& byteRegister);
	void ShiftLeftLogicalMemory(uint16_t address);
	void ShiftRightLogical(uint8_t& byteRegister);
	void ShiftRightLogicalMemory(uint16_t address);
	void RotateDigitRight(uint16_t address);
	void DecimalAdjust(uint8_t& byteRegister);
	void LoadWord(uint16_t& wordRegister, uint16_t address);
	void LoadWordFromMemory(uint16_t& wordRegister);
	void StoreWord(uint16_t address, uint16_t wordRegister);
	void StoreWordToMemory(uint16_t wordRegister);
	void AddWordOffset(uint16_t& destination, int offset) { destination += offset; }
	void AddWord(uint16_t& destination, uint16_t value);
	void AddWordCarry(uint16_t& destination, uint16_t value);
	void SubtractWordCarry(uint16_t& destination, const uint16_t value);
	void ExchangeStack(uint16_t& wordRegister);
	void ExchangeRegisters(uint16_t& wordRegister1, uint16_t& wordRegister2);
	void JumpAbsolute(uint16_t address);
	void JumpAbsolute();
	void JumpAbsolute(std::function<bool(uint8_t)> condition);
	void JumpRelative();
	void JumpRelative(std::function<bool(uint8_t)> condition);
	void DecrementJump(uint8_t& byteRegister);
	void Return();
	void Return(std::function<bool(uint8_t)> condition);
	void Call(uint16_t address);
	void Call();
	void Call(const std::function<bool(uint8_t)>& condition);
	void WriteIo(uint16_t address, uint8_t byteRegister);
	void WriteIo(uint8_t byteRegister);
	void ReadIo(uint8_t& byteRegister, uint16_t address);
	void ReadIo(uint8_t& byteRegister);
	void TestBit(int bit, uint8_t byteRegister);
	void TestBitMemory(int bit, uint16_t address);
	void ResetBit(int bit, uint8_t& byteRegister);
	void ResetBitMemory(int bit, uint16_t address);
	void SetBit(int bit, uint8_t& byteRegister);
	void SetBitMemory(int bit, uint16_t address);
public:
	Z80Cpu(MemorySpace* pMemorySpace, MemorySpace* pIoSpace) : pMemorySpace(pMemorySpace), pIoSpace(pIoSpace) {}
	void Reset();
	void OnClock(uint32_t time) override;
};
