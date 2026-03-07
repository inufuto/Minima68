#include "Cpu6800.h"

#include <cassert>

static auto Undefined = "???";

static const std::function<void(Cpu6800&)> Nop = [](Cpu6800& cpu) {};
int ShiftRightLogical(int _cpp_par_);
int RotateRight(int _cpp_par_);
int ShiftRightArithmetic(int _cpp_par_);
const Cpu6800::Instruction Cpu6800::Instructions[] = {
	{ 2, Undefined,Nop}, // 00
	{2,"NOP",Nop}, // 01 NOP
	{2, Undefined,Nop}, // 02
	{2, Undefined,Nop}, // 03
	{2, Undefined,Nop}, // 04
	{2, Undefined,Nop}, // 05
	{2, "TAP",[](Cpu6800& cpu)
	{
		// 06 TAP
		cpu.cc = cpu.a;
	}},
	{2,"TPA",[](Cpu6800& cpu)
	{
		// 07 TPA
		cpu.a = cpu.cc | 0b110000;
	}},
	{4,"INX",[](Cpu6800& cpu)
	{
		// 08 INX
		cpu.UpdateFlag(Condition::Zero, ++cpu.x == 0);
	}},
	{4,"DEX",[](Cpu6800& cpu)
	{
		// 09 DEX
		cpu.UpdateFlag(Condition::Zero, --cpu.x == 0);
	}},
	{2,"CLV",[](Cpu6800& cpu)
	{
		// 0a CLV
		cpu.ClearFlag(Condition::Overflow);
	}},
	{2,"SEV",[](Cpu6800& cpu)
	{
		// 0b SEV
		cpu.SetFlag(Condition::Overflow);
	}},
	{2,"CLC",[](Cpu6800& cpu)
	{
		// 0c CLC
		cpu.ClearFlag(Condition::Carry);
	}},
	{2,"SEC",[](Cpu6800& cpu)
	{
		// 0d SEC
		cpu.SetFlag(Condition::Carry);
	}},
	{2,"CLI",[](Cpu6800& cpu)
	{
		// 0e CLI
		cpu.ClearFlag(Condition::InterruptDisable);
	}},
	{2,"SEI",[](Cpu6800& cpu)
	{
		// 0f SEI
		cpu.SetFlag(Condition::InterruptDisable);
	}},
	{2,"SBA",[](Cpu6800& cpu)
	{
		// 10 SBA
		cpu.a = cpu.Subtract(cpu.a, cpu.b);
	}},
	{2,"CBA",[](Cpu6800& cpu)
	{
		// 11 CBA
		cpu.Subtract(cpu.a, cpu.b);
	}},
	{2,Undefined,Nop}, // 12
	{2,Undefined,Nop}, // 13
	{2,Undefined,Nop}, // 14
	{2,Undefined,Nop}, // 15
	{2,"TAB",[](Cpu6800& cpu)
	{
		// 16 TAB
		cpu.UpdateFlagsForByte(cpu.b = cpu.a);
	}},
	{2,"TBA",[](Cpu6800& cpu)
	{
		// 17 TBA
		cpu.UpdateFlagsForByte(cpu.a = cpu.b);
	}},
	{2,Undefined,Nop}, // 18
	{2,"DAA",[](Cpu6800& cpu)
	{
		// 19 DAA
		cpu.DecimalAdjust();
	}},
	{2,Undefined,Nop}, // 1a
	{2,"ABA",[](Cpu6800& cpu)
	{
		// 1b ABA
		cpu.a = cpu.Add(cpu.a, cpu.b);
	}},
	{2,Undefined,Nop}, // 1c
	{2,Undefined,Nop}, // 1d
	{2,Undefined,Nop}, // 1e
	{2,Undefined,Nop}, // 1f
	{4,"BRA\tr",[](Cpu6800& cpu)
	{
		// 20 BRA
		cpu.Branch();
	}},
	{ 2,Undefined,Nop }, // 21
	{2 /* or 4*/, "BHI\tr",[](Cpu6800& cpu)
	{
		// 22 BHI
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & (Condition::Carry | Condition::Zero)) == 0;
		});
	}},
	{2 /* or 4 */,"BLS\tr",[](Cpu6800& cpu)
	{
		// 23 BLS
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & (Condition::Carry | Condition::Zero)) != 0;
		});
	}},
	{2 /* or 4 */,"BCC\tr",[](Cpu6800& cpu)
	{
		// 24 BCC
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Carry) == 0;
		});
	}},
	{2 /* or 4 */,"BCS\tr",[](Cpu6800& cpu)
	{
		// 25 BCS
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Carry) != 0;
		});
	}},
	{2 /* or 4 */,"BNE\tr",[](Cpu6800& cpu)
	{
		// 26 BNE
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Zero) == 0;
		});
	}},
	{2 /* or 4 */,"BEQ\tr",[](Cpu6800& cpu)
	{
		// 27 BEQ
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Zero) != 0;
		});
	} },
	{2 /* or 4 */,"BVC\tr",[](Cpu6800& cpu)
	{
		// 28 BVC
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Overflow) == 0;
		});
	}},
	{2 /* or 4 */,"BVS\tr",[](Cpu6800& cpu)
	{
		// 29 BVS
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Overflow) != 0;
		});
	}},
	{2 /* or 4 */,"BPL\tr",[](Cpu6800& cpu)
	{
		// 2a BPL
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Negative) == 0;
		});
	}},
	{2 /* or 4 */,"BMI\tr",[](Cpu6800& cpu)
	{
		// 2b BMI
		cpu.BranchIf([](const uint8_t cc) {
			return (cc & Condition::Negative) != 0;
		});
	} },
	{2 /* or 4 */,"BGE\tr",[](Cpu6800& cpu)
	{
		// 2c BGE (N XOR V = 0)
		cpu.BranchIf([](const uint8_t cc) {
			bool n = (cc & Condition::Negative) != 0;
			bool v = (cc & Condition::Overflow) != 0;
			return n == v;
		});
	}},
	{2 /* or 4 */,"BLT\tr",[](Cpu6800& cpu)
	{
		// 2d BLT (N XOR V = 1)
		cpu.BranchIf([](const uint8_t cc) {
			bool n = (cc & Condition::Negative) != 0;
			bool v = (cc & Condition::Overflow) != 0;
			return n != v;
		});
	}},
	{2 /* or 4 */,"BGT\tr",[](Cpu6800& cpu)
	{
		// 2e BGT (Z + (N XOR V) = 0)
		cpu.BranchIf([](const uint8_t cc) {
			bool z = (cc & Condition::Zero) != 0;
			bool n = (cc & Condition::Negative) != 0;
			bool v = (cc & Condition::Overflow) != 0;
			return !z && (n == v);
		});
	}},
	{2 /* or 4 */,"BLE\tr",[](Cpu6800& cpu)
	{
		// 2f BLE (Z + (N XOR V) = 1)
		cpu.BranchIf([](const uint8_t cc) {
			bool z = (cc & Condition::Zero) != 0;
			bool n = (cc & Condition::Negative) != 0;
			bool v = (cc & Condition::Overflow) != 0;
			return z || (n != v);
		});
	} },
	{4,"TSX",[](Cpu6800& cpu)
	{
		// 30 TSX
		cpu.x = cpu.sp + 1;
	}},
	{4,"INS",[](Cpu6800& cpu)
	{
		// 31 INS
		++cpu.sp;
	} },
	{4,"PULA",[](Cpu6800& cpu)
	{
		// 32 PULA
		cpu.a = cpu.Pull();
	}},
	{4,"PULB",[](Cpu6800& cpu)
	{
		// 33 PULB
		cpu.b = cpu.Pull();
	}},
	{4,"DES",[](Cpu6800& cpu)
	{
		// 34 DES
		--cpu.sp;
	} },
	{4,"TXS",[](Cpu6800& cpu)
	{
		// 35 TXS
		cpu.sp = cpu.x - 1;
	}},
	{4,"PSHA",[](Cpu6800& cpu)
	{
		// 36 PSHA
		cpu.Push(cpu.a);
	}},
	{4,"PSHB",[](Cpu6800& cpu)
	{
		// 37 PSHB
		cpu.Push(cpu.b);
	} },
	{ 2,Undefined,Nop }, // 38
	{5,"RTS",[](Cpu6800& cpu)
	{
		// 39 RTS
		cpu.ReturnFromSubroutine();
	}},
	{ 2,Undefined,Nop }, // 3a
	{ 10,"RTI",[](Cpu6800& cpu)
	{
		// 3b RTI
		cpu.ReturnFromInterrupt();
	}},
	{ 2,Undefined,Nop }, // 3c
	{ 2,Undefined,Nop }, // 3d
	{9,"WAI",[](Cpu6800& cpu)
	{
		// 3e WAI
		cpu.WaitForInterrupt();
	}},
	{12,"SWI",[](Cpu6800& cpu)
	{
		// 3f SWI
		cpu.Interrupt(0xfffa);
	}},
	{2,"NEGA",[](Cpu6800& cpu)
	{
		// 40 NEGA
		cpu.a = cpu.Negate(cpu.a);
	}},
	{ 2,Undefined,Nop }, // 41
		{ 2,Undefined,Nop }, // 42
	{2,"COMA",[](Cpu6800& cpu)
	{
		// 43 COMA
		cpu.a = cpu.Complement(cpu.a);
	}},
	{2,"LSRA",[](Cpu6800& cpu)
	{
		// 44 LSRA
		cpu.a = cpu.ShiftRightLogical(cpu.a);
	}},
	{ 2,Undefined,Nop }, // 45
	{2,"RORA",[](Cpu6800& cpu)
	{
		// 46 RORA
		cpu.a = cpu.RotateRight(cpu.a);
	}},
	{2,"ASRA",[](Cpu6800& cpu)
	{
		// 47 ASRA
		cpu.a = cpu.ShiftRightArithmetic(cpu.a);
	}},
	{2,"ASLA",[](Cpu6800& cpu)
	{
		// 48 ASLA
		cpu.a = cpu.ShiftLeftLogical(cpu.a);
	}},
	{2,"ROLA",[](Cpu6800& cpu)
	{
		// 49 ROLA
		cpu.a = cpu.RotateLeft(cpu.a);
	}},
	{2,"DECA",[](Cpu6800& cpu)
	{
		// 4a DECA
		cpu.a = cpu.Decrement(cpu.a);
	}},
	{ 2,Undefined,Nop }, // 4b
	{2,"INCA",[](Cpu6800& cpu)
	{
		// 4c INCA
		cpu.a = cpu.Increment(cpu.a);
	}},
	{2,"TSTA",[](Cpu6800& cpu)
	{
		// 4d TSTA
		cpu.a = cpu.Test(cpu.a);
	}},
	{ 2,Undefined,Nop }, // 4e
	{2,"CLRA",[](Cpu6800& cpu)
	{
		// 4f CLRA
		cpu.a = cpu.Clear(cpu.a);
	}},
	{2,"NEGB",[](Cpu6800& cpu)
	{
		// 50 NEGB
		cpu.b = cpu.Negate(cpu.b);
	}},
	{ 2,Undefined,Nop }, // 51
	{ 2,Undefined,Nop }, // 52
	{2,"COMB",[](Cpu6800& cpu)
	{
		// 53 COMB
		cpu.b = cpu.Complement(cpu.b);
	}},
	{2,"LSRB",[](Cpu6800& cpu)
	{
		// 54 LSRB
		cpu.b = cpu.ShiftRightLogical(cpu.b);
	}},
	{ 2,Undefined,Nop }, // 55
	{2,"RORB",[](Cpu6800& cpu)
	{
		// 56 RORB
		cpu.b = cpu.RotateRight(cpu.b);
	}},
	{2,"ASRB",[](Cpu6800& cpu)
	{
		// 57 ASRB
		cpu.b = cpu.ShiftRightArithmetic(cpu.b);
	}},
	{2,"ASLB",[](Cpu6800& cpu)
	{
		// 58 ASLB
		cpu.b = cpu.ShiftLeftLogical(cpu.b);
	}},
	{2,"ROLB",[](Cpu6800& cpu)
	{
		// 59 ROLB
		cpu.b = cpu.RotateLeft(cpu.b);
	}},
	{2,"DECB",[](Cpu6800& cpu)
	{
		// 5a DECB
		cpu.b = cpu.Decrement(cpu.b);
	}},
	{ 2,Undefined,Nop }, // 5b
	{2,"INCB",[](Cpu6800& cpu)
	{
		// 5c INCB
		cpu.b = cpu.Increment(cpu.b);
	} },
	{2,"TSTB",[](Cpu6800& cpu)
	{
		// 5d TSTB
		cpu.b = cpu.Test(cpu.b);
	}},
	{ 2,Undefined,Nop }, // 5e
	{2,"CLRB",[](Cpu6800& cpu)
	{
		// 5f CLRB
		cpu.b = cpu.Clear(cpu.b);
	} },
	{7,"NEG\tb,X",[](Cpu6800& cpu)
	{
		// 60 NEG b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.Negate(value);
		});
	}},
	{ 7,Undefined,Nop }, // 61
	{ 7,Undefined,Nop }, // 62
	{7,"COM\tb,X",[](Cpu6800& cpu)
	{
		// 63 COM b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.Complement(value);
		});
	}},
	{7,"LSR\tb,X",[](Cpu6800& cpu)
	{
		// 64 LSR b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.ShiftRightLogical(value);
		});
	}},
	{ 7,Undefined,Nop }, // 65
	{7,"ROR\tb,X",[](Cpu6800& cpu)
	{
		// 66 ROR b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.RotateRight(value);
		});
	} },
	{7,"ASR\tb,X",[](Cpu6800& cpu)
	{
		// 67 ASR b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.ShiftRightArithmetic(value);
		});
	}},
	{7,"ASL\tb,X",[](Cpu6800& cpu)
	{
		// 68 ASL b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.ShiftLeftLogical(value);
		});
	}},
	{7,"ROL\tb,X",[](Cpu6800& cpu)
	{
		// 69 ROL b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.RotateLeft(value);
		});
	} },
	{7,"DEC\tb,X",[](Cpu6800& cpu)
	{
		// 6a DEC b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.Decrement(value);
		});
	}},
	{ 7,Undefined,Nop }, // 6b
	{7,"INC\tb,X",[](Cpu6800& cpu)
	{
		// 6c INC b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.Increment(value);
		});
	}},
	{7,"TST\tb,X",[](Cpu6800& cpu)
	{
		// 6d TST b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.Test(value);
		});
	}},
	{7,"JMP\tb,X",[](Cpu6800& cpu)
	{
		// 6e JMP b,X
		cpu.Jump(cpu.IndexedAddress());
	}},
	{7,"CLR\tb,X",[](Cpu6800& cpu)
	{
		// 6f CLR b,X
		cpu.IndexedOperation([&](uint8_t value) {
			return cpu.Clear(value);
		});
	}},
	{6,"NEG\tw",[](Cpu6800& cpu)
	{
		// 70 NEG w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.Negate(value);
		});
	}},
	{ 6,Undefined,Nop }, // 71
	{ 6,Undefined,Nop }, // 72
	{6,"COM\tw",[](Cpu6800& cpu)
	{
		// 73 COM w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.Complement(value);
		});
	}},
	{6,"LSR\tw",[](Cpu6800& cpu)
	{
		// 74 LSR w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.ShiftRightLogical(value);
		});
	}},
	{ 6,Undefined,Nop }, // 75
	{6,"ROR\tw",[](Cpu6800& cpu)
	{
		// 76 ROR w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.RotateRight(value);
		});
	}},
	{6,"ASR\tw",[](Cpu6800& cpu)
	{
		// 77 ASR w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.ShiftRightArithmetic(value);
		});
	}},
	{6,"ASL\tw",[](Cpu6800& cpu)
	{
		// 78 ASL w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.ShiftLeftLogical(value);
		});
	}},
	{6,"ROL\tw",[](Cpu6800& cpu)
	{
		// 79 ROL w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.RotateLeft(value);
		});
	} },
	{6,"DEC\tw",[](Cpu6800& cpu)
	{
		// 7a DEC w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.Decrement(value);
		});
	}},
	{ 6,Undefined,Nop }, // 7b
	{6,"INC\tw",[](Cpu6800& cpu)
	{
		// 7c INC w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.Increment(value);
		});
	}},
	{6,"TST\tw",[](Cpu6800& cpu)
	{
		// 7d TST w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.Test(value);
		});
	}},
	{6,"JMP\tw",[](Cpu6800& cpu)
	{
		// 7e JMP w
		cpu.Jump(cpu.FetchWord());
	}},
	{6,"CLR\tw",[](Cpu6800& cpu)
	{
		// 7f CLR w
		cpu.ExtendedOperation([&](uint16_t value) {
			return cpu.Clear(value);
		});
	} },
	{2,"SUBA\t#b",[](Cpu6800& cpu)
	{
		// 80 SUBA #b
		cpu.a = cpu.Subtract(cpu.a, cpu.FetchByte());
	}},
	{2,"CMPA\t#b",[](Cpu6800& cpu)
	{
		// 81 CMPA #b
		cpu.Subtract(cpu.a, cpu.FetchByte());
	}},
	{2,"SBCA\t#b",[](Cpu6800& cpu)
	{
		//	82 SBCA #b
		cpu.a = cpu.SubtractWithCarry(cpu.a, cpu.FetchByte());
	}},
	{ 6,Undefined,Nop }, // 83
	{2,"ANDA\t#b",[](Cpu6800& cpu)
	{
		// 84 ANDA #b
		cpu.a = cpu.And(cpu.a, cpu.FetchByte());
	}},
	{2,"BITA\t#b",[](Cpu6800& cpu)
	{
		// 85 BITA #b
		cpu.And(cpu.a, cpu.FetchByte());
	}},
	{2,"LDAA\t#b",[](Cpu6800& cpu)
	{
		// 86 LDAA #b
		cpu.a = cpu.Load(cpu.FetchByte());
	}},
	{ 6,Undefined,Nop }, // 87
	{2,"EORA\t#b",[](Cpu6800& cpu)
	{
		// 88 EORA #b
		cpu.a = cpu.Xor(cpu.a, cpu.FetchByte());
	}},
	{2,"ADCA\t#b",[](Cpu6800& cpu)
	{
		// 89 ADCA #b
		cpu.a = cpu.AddWithCarry(cpu.a, cpu.FetchByte());
	}},
	{2,"ORAA\t#b",[](Cpu6800& cpu)
	{
		// 8a ORAA #b
		cpu.a = cpu.Or(cpu.a, cpu.FetchByte());
	}},
	{ 2,"ADDA\t#b",[](Cpu6800& cpu) {
		// 8b ADDA #b
		cpu.a = cpu.Add(cpu.a, cpu.FetchByte());
	} },
	{4,"CPX\t#w",[](Cpu6800& cpu)
	{
		// 8c CPX #w
		cpu.CompareX(cpu.FetchWord());
	}},
	{8,"BSR\t#r",[](Cpu6800& cpu)
	{
		// 8d BSR #r
		cpu.BranchToSubroutine();
	}},
	{3,"LDS\t#w",[](Cpu6800& cpu)
	{
		// 8e LDS #w
		cpu.UpdateFlagsForWord(cpu.sp = cpu.FetchWord());
	}},
	{ 6,Undefined,Nop }, // 8f
	{3,"SUBA\tb",[](Cpu6800& cpu)
	{
		// 90 SUBA b
		cpu.a = cpu.Subtract(cpu.a, cpu.LoadDirect());
	}},
	{3,"CMPA\tb",[](Cpu6800& cpu)
	{
		// 91 CMPA b
		cpu.Subtract(cpu.a, cpu.LoadDirect());
	}},
	{3,"SBCA\tb",[](Cpu6800& cpu)
	{
		// 92 SBCA b
		cpu.a = cpu.SubtractWithCarry(cpu.a, cpu.LoadDirect());
	}},
	{ 6,Undefined,Nop }, // 93
	{3,"ANDA\tb",[](Cpu6800& cpu)
	{
		// 94 ANDA b
		cpu.a = cpu.And(cpu.a, cpu.LoadDirect());
	}},
	{3,"BITA\tb",[](Cpu6800& cpu)
	{
		// 95 BITA b
		cpu.And(cpu.a, cpu.LoadDirect());
	}},
	{3,"LDAA\tb",[](Cpu6800& cpu)
	{
		// 96 LDAA b
		cpu.a = cpu.LoadDirect();
	}},
	{4,"STAA\tb",[](Cpu6800& cpu)
	{
		// 97 STAA b
		cpu.StoreDirect(cpu.a);
	}},
	{3,"EORA\tb",[](Cpu6800& cpu)
	{
		// 98 EORA b
		cpu.a = cpu.Xor(cpu.a, cpu.LoadDirect());
	}},
	{3,"ADCA\tb",[](Cpu6800& cpu)
	{
		// 99 ADCA b
		cpu.a = cpu.AddWithCarry(cpu.a, cpu.LoadDirect());
	}},
	{3,"ORAA\tb",[](Cpu6800& cpu)
	{
		// 9a ORAA b
		cpu.a = cpu.Or(cpu.a, cpu.LoadDirect());
	}},
	{ 3,"ADDA\tb",[](Cpu6800& cpu) {
		// 9b ADDA b
		cpu.a = cpu.Add(cpu.a, cpu.LoadDirect());
	} },
	{4,"CPX\tb",[](Cpu6800& cpu)
	{
		// 9c CPX b
		cpu.CompareX(cpu.LoadWordDirect());
	}},
	{ 6,Undefined,Nop }, // 9d HCF is not implemented
	{4,"LDS\tb",[](Cpu6800& cpu)
	{
		// 9e LDS b
		cpu.UpdateFlagsForWord(cpu.sp = cpu.LoadWordDirect());
	}},
	{5,"STS\tb",[](Cpu6800& cpu)
	{
		// 9f STS b
		cpu.StoreWordDirect(cpu.sp);
	} },
	{5,"SUBA\tb,X",[](Cpu6800& cpu)
	{
		// a0 SUBA b,X
		cpu.a = cpu.Subtract(cpu.a, cpu.LoadIndexed());
	}},
		{5,"CMPA\tb,X",[](Cpu6800& cpu)
	{
		// a1 CMPA b,X
		cpu.Subtract(cpu.a, cpu.LoadIndexed());
	}},
	{5,"SBCA\tb,X",[](Cpu6800& cpu)
	{
		// a2 SBCA b,X
		cpu.a = cpu.SubtractWithCarry(cpu.a, cpu.LoadIndexed());
	}},
	{ 6,Undefined,Nop }, // a3
	{5,"ANDA\tb,X",[](Cpu6800& cpu)
	{
		// a4 ANDA b,X
		cpu.a = cpu.And(cpu.a, cpu.LoadIndexed());
	}},
	{5,"BITA\tb,X",[](Cpu6800& cpu)
	{
		// a5 BITA b,X
		cpu.And(cpu.a, cpu.LoadIndexed());
	}},
	{5,"LDAA\tb,X",[](Cpu6800& cpu)
	{
		// a6 LDAA b,X
		cpu.a = cpu.LoadIndexed();
	}},
	{6,"STAA\tb,X",[](Cpu6800& cpu)
	{
		// a7 STAA b,X
		cpu.StoreIndexed(cpu.a);
	}},
	{5,"EORA\tb,X",[](Cpu6800& cpu)
	{
		// a8 EORA b,X
		cpu.a = cpu.Xor(cpu.a, cpu.LoadIndexed());
	}},
	{5,"ADCA\tb,X",[](Cpu6800& cpu)
	{
		// a9 ADCA b,X
		cpu.a = cpu.AddWithCarry(cpu.a, cpu.LoadIndexed());
	}},
	{5,"ORAA\tb,X",[](Cpu6800& cpu)
	{
		// aa ORAA b,X
		cpu.a = cpu.Or(cpu.a, cpu.LoadIndexed());
	}},
	{ 5,"ADDA\tb,X",[](Cpu6800& cpu) {
		// ab ADDA b,X
		cpu.a = cpu.Add(cpu.a, cpu.LoadIndexed());
	} },
	{6,"CPX\tb,X",[](Cpu6800& cpu)
	{
		// ac CPX b,X
		cpu.CompareX(cpu.LoadWordIndexed());
	}},
	{8,"JSR\tb,X",[](Cpu6800& cpu)
	{
		// ad JSR b,X
		cpu.JumpToSubroutine(cpu.IndexedAddress());
	} },
	{6,"LDS\tb,X",[](Cpu6800& cpu)
	{
		// ae LDS b,X
		cpu.UpdateFlagsForWord(cpu.sp = cpu.LoadWordIndexed());
	}},
	{6,"STS\tb,X",[](Cpu6800& cpu)
	{
		// af STS b,X
		cpu.StoreWordIndexed(cpu.sp);
	} },
	{4,"SUBA\tw",[](Cpu6800& cpu)
	{
		//	b0 SUBA w
		cpu.a = cpu.Subtract(cpu.a, cpu.LoadExtended());
	}},
	{4,"CMPA\tw",[](Cpu6800& cpu)
	{
		// b1 CMPA w
		cpu.Subtract(cpu.a, cpu.LoadExtended());
	}},
	{4,"SBCA\tw",[](Cpu6800& cpu)
	{
		// b2 SBCA w
		cpu.a = cpu.SubtractWithCarry(cpu.a, cpu.LoadExtended());
	}},
	{ 6,Undefined,Nop }, // b3
	{4,"ANDA\tw",[](Cpu6800& cpu)
	{
		// b4 ANDA w
		cpu.a = cpu.And(cpu.a, cpu.LoadExtended());
	}},
	{4,"BITA\tw",[](Cpu6800& cpu)
	{
		// b5 BITA w
		cpu.And(cpu.a, cpu.LoadExtended());
	}},
	{4,"LDAA\tw",[](Cpu6800& cpu)
	{
		// b6 LDAA w
		cpu.a = cpu.LoadExtended();
	}},
	{5,"STAA\tw",[](Cpu6800& cpu)
	{
		// b7 STAA w
		cpu.StoreExtended(cpu.a);
	} },
	{4,"EORA\tw",[](Cpu6800& cpu)
	{
		// b8 EORA w
		cpu.a = cpu.Xor(cpu.a, cpu.LoadExtended());
	}},
	{4,"ADCA\tw",[](Cpu6800& cpu)
	{
		// b9 ADCA w
		cpu.a = cpu.AddWithCarry(cpu.a, cpu.LoadExtended());
	}},
	{4,"ORAA\tw",[](Cpu6800& cpu)
	{
		// ba ORAA w
		cpu.a = cpu.Or(cpu.a, cpu.LoadExtended());
	}},
	{ 4,"ADDA\tw",[](Cpu6800& cpu) {
		// bb ADDA w
		cpu.a = cpu.Add(cpu.a, cpu.LoadExtended());
	} },
	{5,"CPX\tw",[](Cpu6800& cpu)
	{
		// bc CPX w
		cpu.CompareX(cpu.LoadExtended());
	}},
	{ 6,Undefined,Nop }, // bd
	{5,"LDS\tw",[](Cpu6800& cpu)
	{
		// be LDS w
		cpu.UpdateFlagsForWord(cpu.sp = cpu.LoadExtended());
	}},
	{5,"STS\tw",[](Cpu6800& cpu)
	{
		// bf STS w
		cpu.StoreWordExtended(cpu.sp);
	} },
	{2,"SUBB\t#b",[](Cpu6800& cpu)
	{
		// c0 SUBB #b
		cpu.b = cpu.Subtract(cpu.b, cpu.FetchByte());
	}},
	{2,"CMPB\t#b",[](Cpu6800& cpu)
	{
		// c1 CMPB #b
		cpu.Subtract(cpu.b, cpu.FetchByte());
	}},
	{2,"SBCB\t#b",[](Cpu6800& cpu)
	{
		// c2 SBCB #b
		cpu.b = cpu.SubtractWithCarry(cpu.b, cpu.FetchByte());
	}},
	{ 6,Undefined,Nop }, // c3
	{2,"ANDB\t#b",[](Cpu6800& cpu)
	{
		// c4 ANDB #b
		cpu.b = cpu.And(cpu.b, cpu.FetchByte());
	}},
	{2,"BITB\t#b",[](Cpu6800& cpu)
	{
		// c5 BITB #b
		cpu.And(cpu.b, cpu.FetchByte());
	}},
	{2,"LDB\t#b",[](Cpu6800& cpu)
	{
		// c6 LDB #b
		cpu.b = cpu.Load(cpu.FetchByte());
	}},
	{ 6,Undefined,Nop }, // c7
	{2,"EORB\t#b",[](Cpu6800& cpu)
	{
		// c8 EORB #b
		cpu.b = cpu.Xor(cpu.b, cpu.FetchByte());
	}},
	{2,"ADCB\t#b",[](Cpu6800& cpu)
	{
		// c9 ADCB #b
		cpu.b = cpu.AddWithCarry(cpu.b, cpu.FetchByte());
	}},
	{2,"ORAB\t#b",[](Cpu6800& cpu)
	{
		// ca ORAB #b
		cpu.b = cpu.Or(cpu.b, cpu.FetchByte());
	} },
	{ 2,"ADDB\t#b",[](Cpu6800& cpu) {
		// cb ADDB #b
		cpu.b = cpu.Add(cpu.b, cpu.FetchByte());
	} },
	{ 6,Undefined,Nop }, // cc
	{ 6,Undefined,Nop }, // cd
	{3,"LDS\t#w",[](Cpu6800& cpu)
	{
		// ce LDS #w
		cpu.UpdateFlagsForWord(cpu.sp = cpu.FetchWord());
	}},
	{ 6,Undefined,Nop }, // cf
	{3,"SUBB\tb",[](Cpu6800& cpu)
	{
		// d0 SUBB b
		cpu.b = cpu.Subtract(cpu.b, cpu.LoadDirect());
	}},
	{3,"CMPB\tb",[](Cpu6800& cpu)
	{
		// d1 CMPB b
		cpu.Subtract(cpu.b, cpu.LoadDirect());
	}},
	{3,"SBCB\tb",[](Cpu6800& cpu)
	{
		// d2 SBCB b
		cpu.b = cpu.SubtractWithCarry(cpu.b, cpu.LoadDirect());
	}},
	{ 6,Undefined,Nop }, // d3
	{3,"ANDB\tb",[](Cpu6800& cpu)
	{
		// d4 ANDB b
		cpu.b = cpu.And(cpu.b, cpu.LoadDirect());
	}},
	{3,"BITB\tb",[](Cpu6800& cpu)
	{
		// d5 BITB b
		cpu.And(cpu.b, cpu.LoadDirect());
	}},
	{3,"LDB\tb",[](Cpu6800& cpu)
	{
		// d6 LDB b
		cpu.b = cpu.LoadDirect();
	}},
	{4,"STB\tb",[](Cpu6800& cpu)
	{
		// d7 STB b
		cpu.StoreDirect(cpu.b);
	} },
	{3,"EORB\tb",[](Cpu6800& cpu)
	{
		// d8 EORB b
		cpu.b = cpu.Xor(cpu.b, cpu.LoadDirect());
	}},
	{3,"ADCB\tb",[](Cpu6800& cpu)
	{
		// d9 ADCB b
		cpu.b = cpu.AddWithCarry(cpu.b, cpu.LoadDirect());
	}},
	{3,"ORAB\tb",[](Cpu6800& cpu)
	{
		// da ORAB b
		cpu.b = cpu.Or(cpu.b, cpu.LoadDirect());
	}},
	{ 3,"ADDB\tb",[](Cpu6800& cpu) {
		// db ADDB b
		cpu.b = cpu.Add(cpu.b, cpu.LoadDirect());
	} },
	{ 6,Undefined,Nop }, // dc
	{ 6,Undefined,Nop }, // dd HCF is not implemented
	{4,"LDX\tb",[](Cpu6800& cpu)
	{
		// de LDX b
		cpu.UpdateFlagsForWord(cpu.x = cpu.FetchWord());
	}},
	{5,"STX\tb",[](Cpu6800& cpu)
	{
		// df STX b
		cpu.StoreWordDirect(cpu.x);
	} },
	{5,"SUBB\tb,X",[](Cpu6800& cpu)
	{
		// e0 SUBB b,X
		cpu.b = cpu.Subtract(cpu.b, cpu.LoadIndexed());
	}},
	{5,"CMPB\tb,X",[](Cpu6800& cpu)
	{
		// e1 CMPB b,X
		cpu.Subtract(cpu.b, cpu.LoadIndexed());
	}},
	{5,"SBCB\tb,X",[](Cpu6800& cpu)
	{
		// e2 SBCB b,X
		cpu.b = cpu.SubtractWithCarry(cpu.b, cpu.LoadIndexed());
	}},
	{ 6,Undefined,Nop }, // e3
	{5,"ANDB\tb,X",[](Cpu6800& cpu)
	{
		// e4 ANDB b,X
		cpu.b = cpu.And(cpu.b, cpu.LoadIndexed());
	}},
	{5,"BITB\tb,X",[](Cpu6800& cpu)
	{
		// e5 BITB b,X
		cpu.And(cpu.b, cpu.LoadIndexed());
	}},
	{5,"LDB\tb,X",[](Cpu6800& cpu)
	{
		// e6 LDB b,X
		cpu.b = cpu.LoadIndexed();
	}},
	{6,"STB\tb,X",[](Cpu6800& cpu)
	{
		// e7 STB b,X
		cpu.StoreIndexed(cpu.b);
	} },
	{5,"EORB\tb,X",[](Cpu6800& cpu)
	{
		// e8 EORB b,X
		cpu.b = cpu.Xor(cpu.b, cpu.LoadIndexed());
	}},
	{5,"ADCB\tb,X",[](Cpu6800& cpu)
	{
		// e9 ADCB b,X
		cpu.b = cpu.AddWithCarry(cpu.b, cpu.LoadIndexed());
	}},
	{5,"ORAB\tb,X",[](Cpu6800& cpu)
	{
		// ea ORAB b,X
		cpu.b = cpu.Or(cpu.b, cpu.LoadIndexed());
	}},
	{ 5,"ADDB\tb,X",[](Cpu6800& cpu) {
		// eb ADDB b,X
		cpu.b = cpu.Add(cpu.b, cpu.LoadIndexed());
	} },
	 { 6,Undefined,Nop }, // ec
	 { 6,Undefined,Nop }, // ed
	{6,"LDX\tb,X",[](Cpu6800& cpu)
	{
		// ee LDX b,X
		cpu.UpdateFlagsForWord(cpu.x = cpu.LoadWordIndexed());
	}},
	{6,"STS\tb,X",[](Cpu6800& cpu)
	{
		// ef STS b,X
		cpu.StoreWordIndexed(cpu.x);
	} },
	{4,"SUBB\tw",[](Cpu6800& cpu)
	{
		// f0 SUBB w
		cpu.b = cpu.Subtract(cpu.b, cpu.LoadExtended());
	}},
	{4,"CMPB\tw",[](Cpu6800& cpu)
	{
		// f1 CMPB w
		cpu.Subtract(cpu.b, cpu.LoadExtended());
	}},
	{4,"SBCB\tw",[](Cpu6800& cpu)
	{
		// f2 SBCB w
		cpu.b = cpu.SubtractWithCarry(cpu.b, cpu.LoadExtended());
	}},
	{ 6,Undefined,Nop }, // f3
	{4,"ANDB\tw",[](Cpu6800& cpu)
	{
		// f4 ANDB w
		cpu.b = cpu.And(cpu.b, cpu.LoadExtended());
	}},
	{4,"BITB\tw",[](Cpu6800& cpu)
	{
		// f5 BITB w
		cpu.And(cpu.b, cpu.LoadExtended());
	}},
	{4,"LDB\tw",[](Cpu6800& cpu)
	{
		// f6 LDB w
		cpu.b = cpu.LoadExtended();
	}},
	{5,"STB\tw",[](Cpu6800& cpu)
	{
		// f7 STB w
		cpu.StoreExtended(cpu.b);
	} },
	{4,"EORB\tw",[](Cpu6800& cpu)
	{
		// f8 EORB w
		cpu.b = cpu.Xor(cpu.b, cpu.LoadExtended());
	}},
	{4,"ADCB\tw",[](Cpu6800& cpu)
	{
		// f9 ADCB w
		cpu.b = cpu.AddWithCarry(cpu.b, cpu.LoadExtended());
	}},
	{4,"ORAB\tw",[](Cpu6800& cpu)
	{
		// fa ORAB w
		cpu.b = cpu.Or(cpu.b, cpu.LoadExtended());
	} },
	  { 4,"ADDB\tw",[](Cpu6800& cpu) {
		// fb ADDB w
		cpu.b = cpu.Add(cpu.b, cpu.LoadExtended());
	} },
	{ 6,Undefined,Nop }, // fc
	{ 6,Undefined,Nop }, // fd
  { 6,"LDX\tw",[](Cpu6800& cpu)
  {
		// fe LDX w
		cpu.UpdateFlagsForWord(cpu.x = cpu.LoadExtended());
	} },
	{ 6,"STX\tw",[](Cpu6800& cpu)
	{
		// ff STX w
		cpu.StoreWordExtended(cpu.x);
	} },
};

const Cpu6800::RegisterView Cpu6800::RegisterViews[] = {
	{ "A",1,
	[](const Cpu6800& cpu) { return cpu.a; },
	[](Cpu6800& cpu, const uint16_t value) { cpu.a = value; }
	},
	{ "B",1,
	[](const Cpu6800& cpu) { return cpu.b; },
	[](Cpu6800& cpu, const uint16_t value) { cpu.b = value; }
	},
	{ "X",2,
	[](const Cpu6800& cpu) { return cpu.x; },
	[](Cpu6800& cpu, const uint16_t value) { cpu.x = value; }
	},
	{ "PC",2,
	[](const Cpu6800& cpu) { return cpu.currentInstructionPc; },
	[](Cpu6800& cpu, const uint16_t value) { cpu.pc = value; }
	},
	{ "SP",2,
	[](const Cpu6800& cpu) { return cpu.sp; },
	[](Cpu6800& cpu, const uint16_t value) { cpu.sp = value; }
	},
	{ "CC",1,
		[](const Cpu6800& cpu) { return cpu.cc; },
		[](Cpu6800& cpu, const uint16_t value) { cpu.cc = value; }
	},
};

void Cpu6800::UpdateFlag(const uint8_t flag, const bool condition)
{
	if (condition) {
		SetFlag(flag);
	}
	else {
		ClearFlag(flag);
	}
}

void Cpu6800::UpdateZeroFlagForByte(uint8_t result)
{
	return UpdateFlag(Condition::Zero, (result & 0xff) == 0);
}

void Cpu6800::UpdateNegativeFlagForByte(uint8_t result)
{
	UpdateFlag(Condition::Negative, (result & 0x80) != 0);
}

void Cpu6800::UpdateFlagsForByte(const uint8_t result)
{
	ClearFlag(Condition::Overflow);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
}

void Cpu6800::UpdateOverflowFlagAfterShift()
{
	if ((cc & Condition::Negative) != 0) {
		UpdateFlag(Condition::Overflow, (cc & Condition::Carry) != 0);
	}
	else {
		UpdateFlag(Condition::Overflow, (cc & Condition::Carry) == 0);
	}
}

uint16_t Cpu6800::FetchWord()
{
	auto high = FetchByte();
	auto low = FetchByte();
	return MakeWord(high, low);
}

void Cpu6800::DecimalAdjust()
{
	uint8_t correction = 0;
	bool carryFlag = false;
	if ((cc & Condition::HalfCarry) != 0 || (a & 0x0f) > 9) {
		correction |= 0x06;
	}
	if ((cc & Condition::Carry) != 0 || (a > 0x99) || (a > 0x8f && (a & 0x0f) > 9)) {
		correction |= 0x60;
		carryFlag = true;
	}
	if ((a & 0x0f) + (correction & 0x0f) >= 0x10) {
		SetFlag(Condition::HalfCarry);
	}
	else {
		ClearFlag(Condition::HalfCarry);
	}
	a += correction;
	if (carryFlag) {
		SetFlag(Condition::Carry);
	}
	UpdateZeroFlagForByte(a);
	UpdateNegativeFlagForByte(a);
}

uint8_t Cpu6800::Subtract(const uint8_t left, const uint8_t right)
{
	auto result = static_cast<uint8_t>(left - right);
	UpdateFlag(Condition::Carry, left < right);
	UpdateFlag(Condition::Overflow, (left ^ right) & (left ^ result) & 0x80);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::SubtractWithCarry(uint8_t left, uint8_t right)
{
	auto carry = (cc & Condition::Carry) != 0 ? 1 : 0;
	auto result = static_cast<uint8_t>(left - right - carry);
	UpdateFlag(Condition::Carry, (static_cast<uint16_t>(right) + carry) > left);
	UpdateFlag(Condition::Overflow, (left ^ right) & (left ^ result) & 0x80);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::Add(const uint8_t left, const uint8_t right)
{
	auto result = static_cast<uint16_t>(left) + right;
	UpdateFlag(Condition::Carry, result > 0xff);
	UpdateFlag(Condition::Overflow, ((left ^ right) & 0x80) == 0 && ((left ^ result) & 0x80) != 0);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	UpdateFlag(Condition::HalfCarry, ((left & 0x0f) + (right & 0x0f)) > 0x0f);
	return result;
}

uint8_t Cpu6800::AddWithCarry(uint8_t left, uint8_t right)
{
	auto carry = (cc & Condition::Carry) != 0 ? 1 : 0;
	auto result = static_cast<uint16_t>(left) + right + carry;
	UpdateFlag(Condition::Carry, result > 0xff);
	UpdateFlag(Condition::Overflow, ((left ^ right) & 0x80) == 0 && ((left ^ result) & 0x80) != 0);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	UpdateFlag(Condition::HalfCarry, ((left & 0x0f) + (right & 0x0f) + carry) > 0x0f);
	return result;
}

uint8_t Cpu6800::And(const uint8_t left, const uint8_t right)
{
	uint8_t result = left & right;
	UpdateFlagsForByte(result);
	return result;
}

uint8_t Cpu6800::Xor(const uint8_t left, const uint8_t right)
{
	auto result = static_cast<uint8_t>(left ^ right);
	UpdateFlagsForByte(result);
	return result;
}

uint8_t Cpu6800::Or(uint8_t left, uint8_t right)
{
	auto result = static_cast<uint8_t>(left | right);
	UpdateFlagsForByte(result);
	return result;
}

uint8_t Cpu6800::Decrement(const uint8_t value)
{
	auto result = static_cast<uint8_t>(value - 1);
	UpdateFlag(Condition::Overflow, value == 0x80);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::Increment(const uint8_t value)
{
	auto result = static_cast<uint8_t>(value + 1);
	UpdateFlag(Condition::Overflow, value == 0x7f);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::Negate(const uint8_t value)
{
	auto result = static_cast<uint8_t>(-value);
	UpdateFlag(Condition::Carry, result != 0);
	UpdateFlag(Condition::Overflow, value == 0x80);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::Complement(const uint8_t value)
{
	auto result = ~value;
	SetFlag(Condition::Carry);
	ClearFlag(Condition::Overflow);
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::Test(uint8_t value)
{
	ClearFlag(Condition::Carry);
	ClearFlag(Condition::Overflow);
	UpdateZeroFlagForByte(value);
	UpdateNegativeFlagForByte(value);
	return value;
}

uint8_t Cpu6800::Clear(uint8_t value)
{
	SetFlag(Condition::Zero);
	ClearFlag(Condition::Carry | Condition::Overflow | Condition::Negative);
	return 0;
}

uint8_t Cpu6800::ShiftRightLogical(const uint8_t value)
{
	auto lowBit = value & 1;
	auto result = static_cast<uint8_t>(value >> 1);
	UpdateFlag(Condition::Carry, lowBit != 0);
	UpdateOverflowFlagAfterShift();
	UpdateZeroFlagForByte(result);
	ClearFlag(Condition::Negative);
	return result;
}

uint8_t Cpu6800::RotateRight(const uint8_t value)
{
	auto lowBit = value & 1;
	auto result = static_cast<uint8_t>((value >> 1) | ((cc & Condition::Carry) != 0 ? 0x80 : 0));
	UpdateFlag(Condition::Carry, lowBit != 0);
	UpdateOverflowFlagAfterShift();
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::ShiftRightArithmetic(const uint8_t value)
{
	auto lowBit = value & 1;
	auto result = static_cast<uint8_t>((value >> 1) | (value & 0x80));
	UpdateFlag(Condition::Carry, lowBit != 0);
	UpdateOverflowFlagAfterShift();
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::ShiftLeftLogical(const uint8_t value)
{
	auto highBit = value & 0x80;
	auto result = static_cast<uint8_t>(value << 1);
	UpdateFlag(Condition::Carry, highBit != 0);
	UpdateOverflowFlagAfterShift();
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::RotateLeft(uint8_t value)
{
	auto highBit = value & 0x80;
	auto result = static_cast<uint8_t>((value << 1) | ((cc & Condition::Carry) != 0 ? 1 : 0));
	UpdateFlag(Condition::Carry, highBit != 0);
	UpdateOverflowFlagAfterShift();
	UpdateZeroFlagForByte(result);
	UpdateNegativeFlagForByte(result);
	return result;
}

uint8_t Cpu6800::Load(const uint8_t source)
{
	UpdateFlagsForByte(source);
	return source;
}

void Cpu6800::CompareX(uint16_t value)
{
	auto result = x - value;
	UpdateFlag(Condition::Overflow, (x ^ value) & (x ^ result) & 0x8000);
	UpdateFlag(Condition::Zero, result == 0);
	UpdateFlag(Condition::Negative, (result & 0x8000) != 0);
}

void Cpu6800::UpdateFlagsForWord(const uint16_t value)
{
	ClearFlag(Condition::Overflow);
	UpdateFlag(Condition::Zero, value == 0);
	UpdateFlag(Condition::Negative, (value & 0x8000) != 0);
}

void Cpu6800::Branch()
{
	auto offset = static_cast<int8_t>(FetchByte());
	pc += offset;
}

void Cpu6800::BranchIf(const std::function<bool(uint8_t)>& condition)
{
	if (condition(cc)) {
		clockCountToExecute += 2;
		pNextInstruction = &Instructions[0x20]; // BRA
	}
	else {
		FetchByte(); // discard offset
	}
}

void Cpu6800::BranchToSubroutine()
{
	auto offset = static_cast<int8_t>(FetchByte());
	JumpToSubroutine(pc + offset);
}

void Cpu6800::ReturnFromSubroutine()
{
	uint8_t high = Pull();
	uint8_t low = Pull();
	pc = MakeWord(high, low);
}

void Cpu6800::ReturnFromInterrupt()
{
	cc = Pull() & 0b110111; // H flag is always cleared
	b = Pull();
	a = Pull();
	uint8_t xh = Pull();
	uint8_t xl = Pull();
	x = MakeWord(xh, xl);
	uint8_t pch = Pull();
	uint8_t pcl = Pull();
	pc = MakeWord(pch, pcl);
}

void Cpu6800::JumpToSubroutine(const uint16_t address)
{
	Push(HighByte(pc));
	Push(LowByte(pc));
	pc = address;
}

uint8_t Cpu6800::Pull()
{
	return pMemorySpace->Read(++sp);
}

void Cpu6800::Push(uint8_t value)
{
	pMemorySpace->Write(sp--, value);
}

void Cpu6800::PushAll() {
	Push(HighByte(pc));
	Push(LowByte(pc));
	Push(HighByte(x));
	Push(LowByte(x));
	Push(a);
	Push(b);
	Push(cc | 0b11000000);
}

void Cpu6800::WaitForInterrupt()
{
	PushAll();
	halted = true;
}

void Cpu6800::Interrupt(const int16_t vectorAddress)
{
	PushAll();
	uint8_t high = pMemorySpace->Read(vectorAddress);
	uint8_t low = pMemorySpace->Read(vectorAddress + 1);
	pc = MakeWord(high, low);
}

uint16_t Cpu6800::IndexedAddress() {
	auto offset = static_cast<int8_t>(FetchByte());
	auto address = static_cast<uint16_t>(x + offset);
	return address;
}

void Cpu6800::OperateMemory(uint16_t address, const std::function<uint8_t(uint8_t)>& operation) const
{
	auto value = pMemorySpace->Read(address);
	auto result = operation(value);
	pMemorySpace->Write(address, result);
}

void Cpu6800::IndexedOperation(const std::function<uint8_t(uint8_t)>& operation)
{
	auto address = IndexedAddress();
	OperateMemory(address, operation);
}

void Cpu6800::ExtendedOperation(const std::function<uint8_t(uint8_t)>& operation)
{
	auto address = FetchWord();
	OperateMemory(address, operation);
}

uint8_t Cpu6800::LoadDirect()
{
	auto address = MakeWord(0, FetchByte());
	return pMemorySpace->Read(address);
}

void Cpu6800::StoreDirect(const uint8_t value)
{
	auto address = MakeWord(0, FetchByte());
	pMemorySpace->Write(address, value);
	UpdateFlagsForByte(value);
}

uint16_t Cpu6800::LoadWordDirect()
{
	auto address = MakeWord(0, FetchByte());
	auto high = pMemorySpace->Read(address);
	auto low = pMemorySpace->Read(address + 1);
	uint16_t word = MakeWord(high, low);
	UpdateFlagsForWord(word);
	return word;
}

void Cpu6800::StoreWordDirect(const uint16_t value)
{
	UpdateFlagsForWord(value);
	auto address = MakeWord(0, FetchByte());
	pMemorySpace->Write(address, HighByte(value));
	pMemorySpace->Write(address + 1, LowByte(value));
}

uint8_t Cpu6800::LoadIndexed()
{
	auto address = IndexedAddress();
	return pMemorySpace->Read(address);
}

void Cpu6800::StoreIndexed(const uint8_t value)
{
	auto address = IndexedAddress();
	pMemorySpace->Write(address, value);
	UpdateFlagsForByte(value);
}

uint16_t Cpu6800::LoadWordIndexed()
{
	auto address = IndexedAddress();
	auto high = pMemorySpace->Read(address);
	auto low = pMemorySpace->Read(address + 1);
	uint16_t word = MakeWord(high, low);
	UpdateFlagsForWord(word);
	return word;
}

void Cpu6800::StoreWordIndexed(const uint16_t value)
{
	auto address = IndexedAddress();
	pMemorySpace->Write(address, HighByte(value));
	pMemorySpace->Write(address + 1, LowByte(value));
	UpdateFlagsForWord(value);
}

uint8_t Cpu6800::LoadExtended()
{
	auto address = FetchWord();
	uint8_t value = pMemorySpace->Read(address);
	return Load(value);
}

void Cpu6800::StoreExtended(const uint8_t value)
{
	auto address = FetchWord();
	pMemorySpace->Write(address, value);
	UpdateFlagsForByte(value);
}

void Cpu6800::StoreWordExtended(uint16_t value)
{
	auto address = FetchWord();
	pMemorySpace->Write(address, HighByte(value));
	pMemorySpace->Write(address + 1, LowByte(value));
	UpdateFlagsForWord(value);
}

void Cpu6800::FetchInstruction()
{
	if (halted) {
		clockCountToExecute = 1;
		return;
	}
	currentInstructionPc = pc;
	uint8_t opcode = FetchByte();
	pNextInstruction = &Instructions[opcode];
	assert(pNextInstruction->clockCount != 0);
	clockCountToExecute = pNextInstruction->clockCount;
	if (GetDebugger() != nullptr && GetDebugger()->PauseRequired(this, currentInstructionPc)) {
		GetDebugger()->Pause();
		return;
	}
}

Cpu6800::Cpu6800(::Debugger* pDebugger, MemorySpace* pMemorySpace) : Cpu(pDebugger),pMemorySpace(pMemorySpace)
{
	static_assert(std::size(Instructions) == 256, "Instructions array must have exactly 256 elements (0x00-0xFF)");
}

void Cpu6800::Reset()
{
	a = b = 0;
	x = 0;
	cc = 0b11000000; // I and H flags are set after reset
	{
		auto high = pMemorySpace->Read(0xfffe);
		auto low = pMemorySpace->Read(0xffff);
		pc = MakeWord(high, low);
	}
	halted = false;
	FetchInstruction();
}

void Cpu6800::ExecuteInstruction() {
	auto pInstruction = pNextInstruction;
	pInstruction->execute(*this);
	if (pNextInstruction == pInstruction) {
		FetchInstruction();
	}
}

void Cpu6800::OnClock(uint32_t time)
{
	if (--clockCountToExecute == 0) {
		ExecuteInstruction();
	}
}

int Cpu6800::GetRegisterCount() const
{
	return std::size(RegisterViews);
}

const char* Cpu6800::GetRegisterName(int index) const
{
	assert(index >= 0 && index < GetRegisterCount());
	return RegisterViews[index].name.c_str();
}

int Cpu6800::GetRegisterSize(int index) const
{
	assert(index >= 0 && index < GetRegisterCount());
	return RegisterViews[index].size;
}

uint16_t Cpu6800::ReadRegister(int index) const
{
	assert(index >= 0 && index < GetRegisterCount());
	return RegisterViews[index].read(*this);
}

void Cpu6800::WriteRegister(int index, uint16_t value)
{
	assert(index >= 0 && index < GetRegisterCount());
	RegisterViews[index].write(*this, value);
}

void Cpu6800::BuildAssemblyElement(uint16_t address, AssemblyElement* pElement) const
{
	auto opcode = pMemorySpace->Read(address);
	const auto& instruction = Instructions[opcode];
	auto pSource = instruction.mnemonic;
	pElement->size = 1;
	pElement->mnemonic.clear();
	char c;
	while ((c = *pSource++) != 0 && c != '\t') {
		pElement->mnemonic += c;
	}
	pElement->operand.clear();
	if (c != 0) {
		while ((c = *pSource++) != 0) {
			switch (c) {
			case 'b': {
				auto byteValue = pMemorySpace->Read(address + 1);
				char byteBuffer[4];
				snprintf(byteBuffer, sizeof(byteBuffer), "$%02X", byteValue);
				pElement->operand += byteBuffer;
				++pElement->size;
				break;
			}
			case 'r': {
				auto offset = static_cast<int8_t>(pMemorySpace->Read(address + 1));
				char relativeBuffer[6];
				snprintf(relativeBuffer, sizeof(relativeBuffer), "$%04X", static_cast<uint16_t>(address + 2 + offset));
				pElement->operand += relativeBuffer;
				++pElement->size;
				break;
			}
			case 'w': {
				auto low = pMemorySpace->Read(address + 1);
				auto high = pMemorySpace->Read(address + 2);
				char wordBuffer[6];
				snprintf(wordBuffer, sizeof(wordBuffer), "$%04X", MakeWord(high, low));
				pElement->operand += wordBuffer;
				pElement->size += 2;
				break;
			}
			default:
				pElement->operand += c;
			}
		}
	}
}

uint16_t Cpu6800::CurrentInstructionSize()
{
	AssemblyElement element;
	BuildAssemblyElement(currentInstructionPc, &element);
	return element.size;
}
