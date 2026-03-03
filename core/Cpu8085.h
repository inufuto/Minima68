#pragma once

#include <functional>

#include "AbtractEmulator.h"
#include "ClockSource.h"
#include "MemorySpace.h"
#include "Uncopyable.h"

class Cpu8085 : public ClockDestination, Uncopyable, public RegisterHolder
{
public:
	static uint16_t MakeWord(uint8_t high, uint8_t low) { return high << 8 | low; }
	static uint8_t HighByte(uint16_t word) { return word >> 8; }
	static uint8_t LowByte(uint16_t word) { return word & 0xff; }
	union PairRegister
	{
		struct {
			uint8_t low;
			uint8_t high;
		};
		uint16_t word;
	};
private:
	static constexpr int RegisterCount = 6;
	struct Flag {
		static constexpr uint8_t Carry = 1 << 0;
		static constexpr uint8_t Overflow = 1 << 1;
		static constexpr uint8_t Parity = 1 << 2;
		static constexpr uint8_t HalfCarry = 1 << 4;
		static constexpr uint8_t X5 = 1 << 5;
		static constexpr uint8_t Zero = 1 << 6;
		static constexpr uint8_t Sign = 1 << 7;
	};
	struct InterruptBit
	{
		static constexpr uint8_t MaskRst55 = 1 << 0;
		static constexpr uint8_t MaskRst65 = 1 << 1;
		static constexpr uint8_t MaskRst75 = 1 << 2;
		static constexpr uint8_t MaskRst = MaskRst55 | MaskRst65 | MaskRst75;
		static constexpr uint8_t MaskIntr = 1 << 3;
		static constexpr uint8_t RequestRst55 = 1 << 4;
		static constexpr uint8_t RequestRst65 = 1 << 5;
		static constexpr uint8_t RequestRst75 = 1 << 6;
		static constexpr uint8_t Sid = 1 << 7;
		static constexpr uint8_t SetRst = 1 << 3;
		static constexpr uint8_t ClearRst75 = 1 << 4;
		static constexpr uint8_t SetSod = 1 << 6;
		static constexpr uint8_t Sod = 1 << 7;
	};
	struct Instruction
	{
		int clockCount;
		const char* mnemonic;
		std::function<void(Cpu8085&)> execute;
	};
	static const Instruction Instructions[];
	struct RegisterView
	{
		std::string name;
		int size;
		std::function<uint16_t (const Cpu8085&)> read;
		std::function<void(Cpu8085&, uint16_t)> write;
	};
	static const RegisterView RegisterViews[];
	std::string name;
	MemorySpace* pMemorySpace;
	MemorySpace* pIoSpace;
	PairRegister af, bc, de, hl;
	uint16_t sp, pc;
	bool interruptsDisabled;
	uint8_t interruptBits;
	const Instruction* pNextInstruction;
	int clockCountToExecute;
	bool halted;
	uint16_t currentInstructionPc;
private:
	uint8_t FetchByte() { return pMemorySpace->Read(pc++); }
	uint16_t FetchWord();
	void PushWord(uint16_t value);
	uint16_t PopWord();
	void FetchInstruction();
	void SetFlag(const uint8_t flag) { af.low |= flag; }
	void ClearFlag(const uint8_t flag) { af.low &= ~flag; }
	bool IsFlagSet(const uint8_t flag) const { return (af.low & flag) != 0; }
	void UpdateZeroSignParity(uint8_t value);
	void UpdateOverflowForAddition(uint8_t destination, uint8_t source, uint8_t result);
	void UpdateOverflowForSubtraction(uint8_t destination, uint8_t source, uint8_t result);

	static void Move(uint8_t& destination, const uint8_t source) { destination = source; }
	void MoveImmediate(uint8_t& byteRegister) { byteRegister = FetchByte(); }
	void MoveImmediateMemory(uint16_t address);
	void LoadByte(uint8_t& byteRegister, const uint16_t address) const { byteRegister = pMemorySpace->Read(address); }
	void LoadByte(uint8_t& byteRegister) { LoadByte(byteRegister, FetchWord()); }
	void StoreByte(const uint16_t address, const uint8_t byteRegister) const { pMemorySpace->Write(address, byteRegister); }
	void StoreByte(const uint8_t byteRegister) { pMemorySpace->Write(FetchWord(), byteRegister); }
	void AddByte(uint8_t& destination, uint8_t source);
	void AddByteMemory(uint8_t& destination, uint16_t address);
	void AddByteImmediate(uint8_t& byteRegister) { AddByte(byteRegister, FetchByte()); }
	void AddByteWithCarry(uint8_t& destination, uint8_t source);
	void AddByteMemoryWithCarry(uint8_t& destination, uint16_t address);
	void AddByteImmediateWithCarry(uint8_t& byteRegister) { AddByteWithCarry(byteRegister, FetchByte()); }
	void SubtractByte(uint8_t& destination, uint8_t source);
	void SubtractByteMemory(uint8_t& destination, uint16_t address);
	void SubtractByteImmediate(uint8_t& byteRegister) { SubtractByte(byteRegister, FetchByte()); }
	void SubtractByteWithCarry(uint8_t& destination, uint8_t source);
	void SubtractByteMemoryWithCarry(uint8_t& destination, uint16_t address);
	void SubtractByteImmediateWithCarry(uint8_t& byteRegister) { SubtractByteWithCarry(byteRegister, FetchByte()); }
	void AndByte(uint8_t& destination, uint8_t source);
	void AndByteMemory(uint8_t& destination, uint16_t address);
	void AndByteImmediate(uint8_t& byteRegister) { AndByte(byteRegister, FetchByte()); }
	void XorByte(uint8_t& destination, uint8_t source);
	void XorByteMemory(uint8_t& destination, uint16_t address);
	void XorByteImmediate(uint8_t& byteRegister) { XorByte(byteRegister, FetchByte()); }
	void OrByte(uint8_t& destination, uint8_t source);
	void OrByteMemory(uint8_t& destination, uint16_t address);
	void OrByteImmediate(uint8_t& byteRegister) { OrByte(byteRegister, FetchByte()); }
	void CompareByte(uint8_t destination, uint8_t source);
	void CompareByteMemory(uint8_t destination, uint16_t source);
	void CompareByteImmediate(uint8_t byteRegister) { CompareByte(byteRegister, FetchByte()); }
	void IncreaseByte(uint8_t& byteRegister, int offset);
	void IncreaseMemory(uint16_t address, int offset);
	void RotateLeftCircular(uint8_t& byteRegister);
	void RotateRightCircular(uint8_t& byteRegister);
	void RotateLeft(uint8_t& byteRegister);
	void RotateRight(uint8_t& byteRegister);
	void DecimalAdjust(uint8_t& byteRegister);
	void LoadWordConst(uint16_t& wordRegister) { wordRegister = FetchWord(); }
	void LoadWordMemory(uint16_t& wordRegister);
	void LoadWord(uint16_t& wordRegister, uint16_t address) const;
	void StoreWord(uint16_t address, uint16_t value);
	void AddWord(uint16_t& wordRegister, uint16_t value);
	void SubtractWord(uint16_t& wordRegister, uint16_t value);
	void IncreaseWord(uint16_t& wordRegister, const int offset);
	void AddWordByteConst(uint16_t& destination, uint16_t source);
	void ShiftWordRightArithmetic(uint16_t& wordRegister);
	void RotateWordLeft(uint16_t& wordRegister);
	static void Exchange(uint16_t& word1, uint16_t& word2);
	void ExchangeStack(uint16_t& wordRegister);
	void ReadInterruptBits(uint8_t& byteRegister) const { byteRegister = interruptBits; }
	void WriteInterruptBits(uint8_t byteRegister);
	void Jump(uint16_t address);
	void ComplementFlag(const uint8_t flag) { af.low ^= flag; }
	void Jump();
	void JumpIf(const std::function<bool(uint8_t)>& condition);
	void Call(uint16_t address);
	void Call() { Call(FetchWord()); }
	void CallIf(const std::function<bool(uint8_t)>& condition);
	void Return();
	void ReturnIf(const std::function<bool(uint8_t)>& condition);
	void RestartIfOverflow();
	void Halt() { halted = true; }
	void Input(uint8_t& byteRegister);
	void Output(uint8_t byteRegister);
public:
	Cpu8085(const char* name, MemorySpace* pMemorySpace, MemorySpace* pIoSpace)
		: name(name), pMemorySpace(pMemorySpace), pIoSpace(pIoSpace) {
	}
	uint16_t ProgramCounter() const { return pc; }
	uint16_t RegisterBc() const { return bc.word; }
	void Reset();
	void OnClock(uint32_t time) override;
	int GetRegisterCount() const override;
	const char* GetRegisterName(int index) const override;
	int GetRegisterSize(int index) const override;
	uint16_t ReadRegister(int index) const override;
	void WriteRegister(int index, uint16_t value) override;
};