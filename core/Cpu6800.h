#pragma once
#include <functional>

#include "Cpu.h"

class Cpu6800 : public Cpu
{
private:
	struct Condition {
		static constexpr uint8_t Carry = 1 << 0;
		static constexpr uint8_t Overflow = 1 << 1;
		static constexpr uint8_t Zero = 1 << 2;
		static constexpr uint8_t Negative = 1 << 3;
		static constexpr uint8_t InterruptDisable = 1 << 4;
		static constexpr uint8_t HalfCarry = 1 << 5;
	};
	struct Instruction {
		int clockCount;
		const char* mnemonic;
		std::function<void(Cpu6800&)> execute;
	};
	static const Instruction Instructions[];
	struct RegisterView
	{
		std::string name;
		int size;
		std::function<uint16_t(const Cpu6800&)> read;
		std::function<void(Cpu6800&, uint16_t)> write;
	};
	static const RegisterView RegisterViews[];
	MemorySpace* pMemorySpace;
	uint8_t a, b, cc;
	uint16_t x, sp, pc;
	const Instruction* pNextInstruction;
	int clockCountToExecute;
	bool halted;
	uint16_t currentInstructionPc;
private:
	void SetFlag(const uint8_t flag) { cc |= flag; }
	void ClearFlag(const uint8_t flag) { cc &= ~flag; }
	void UpdateFlag(uint8_t flag, bool condition);
	bool IsFlagSet(const uint8_t flag) const { return (cc & flag) != 0; }
	void UpdateZeroFlagForByte(uint8_t result);
	void UpdateNegativeFlagForByte(uint8_t result);
	void UpdateFlagsForByte(uint8_t result);
	void UpdateOverflowFlagAfterShift();

	uint8_t FetchByte() { return pMemorySpace->Read(pc++); }
	uint16_t FetchWord();

	void DecimalAdjust();
	uint8_t Subtract(uint8_t left, uint8_t right);
	uint8_t SubtractWithCarry(uint8_t left, uint8_t right);
	uint8_t Add(uint8_t left, uint8_t right);
	uint8_t AddWithCarry(uint8_t left, uint8_t right);
	uint8_t And(uint8_t left, uint8_t right);
	uint8_t Xor(uint8_t left, uint8_t right);
	uint8_t Or(uint8_t left, uint8_t right);
	uint8_t Decrement(uint8_t value);
	uint8_t Increment(uint8_t value);
	uint8_t Negate(uint8_t value);
	uint8_t Complement(uint8_t value);
	uint8_t Test(uint8_t value);
	uint8_t Clear(uint8_t value);
	uint8_t ShiftRightLogical(uint8_t value);
	uint8_t RotateRight(uint8_t value);
	uint8_t ShiftRightArithmetic(uint8_t value);
	uint8_t ShiftLeftLogical(uint8_t value);
	uint8_t RotateLeft(uint8_t value);
	uint8_t Load(uint8_t source);
	void CompareX(uint16_t value);
	void UpdateFlagsForWord(uint16_t value);
	void Branch();
	void BranchIf(const std::function<bool(uint8_t)>& condition);
	void BranchToSubroutine();
	void ReturnFromSubroutine();
	void ReturnFromInterrupt();
	void Jump(const uint16_t address) { pc = address; }
	void JumpToSubroutine(uint16_t address);
	uint8_t Pull();
	void Push(uint8_t value);
	void PushAll();
	void WaitForInterrupt();
	void Interrupt(int16_t vectorAddress);

	uint16_t IndexedAddress();
	void OperateMemory(uint16_t address, const std::function<uint8_t(uint8_t)>& operation) const;
	void IndexedOperation(const std::function<uint8_t(uint8_t)>& operation);
	void ExtendedOperation(const std::function<uint8_t(uint8_t)>& operation);
	uint8_t LoadDirect();
	void StoreDirect(uint8_t value);
	uint16_t LoadWordDirect();
	void StoreWordDirect(uint16_t value);
	uint8_t LoadIndexed();
	void StoreIndexed(uint8_t value);
	uint16_t LoadWordIndexed();
	void StoreWordIndexed(uint16_t value);
	uint8_t LoadExtended();
	void StoreExtended(uint8_t value);
	void StoreWordExtended(uint16_t value);

	void FetchInstruction();
	void ExecuteInstruction();
public:
	Cpu6800(class ::Debugger* pDebugger, MemorySpace* pMemorySpace);

	uint16_t CurrentInstructionAddress() const override { return currentInstructionPc; }
	void Reset() override;
	void OnClock(uint32_t time) override;
	int GetRegisterCount() const override;
	const char* GetRegisterName(int index) const override;
	int GetRegisterSize(int index) const override;
	uint16_t ReadRegister(int index) const override;
	void WriteRegister(int index, uint16_t value) override;
	void BuildAssemblyElement(uint16_t address, AssemblyElement* pElement) const override;
	uint16_t CurrentInstructionSize();
};
