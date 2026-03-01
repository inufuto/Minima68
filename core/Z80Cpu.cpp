#include "Z80Cpu.h"

#include <cassert>

static const std::function<void(Z80Cpu&)> Nop = [](Z80Cpu& z80Cpu) {};
const Z80Cpu::Instruction Z80Cpu::Instructions[] = {
	{ 4, Nop /* 00 */},
	{ 10, [](Z80Cpu& cpu)
	{
		/* 01 LD BC, nn */
		cpu.LoadConstWord(cpu.CurrentExchangeableRegisters().bc.word);
	} },
	{7, [](Z80Cpu& cpu)
	{
		/* 02 LD (BC), A */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().bc.word, cpu.CurrentAf().high);
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 03 INC BC */
		cpu.AddWordOffset(cpu.CurrentExchangeableRegisters().bc.word, 1);
	}},
	{4, [](Z80Cpu& cpu)
	{
		/* 04 INC B */
		cpu.AddByteRegister(cpu.CurrentExchangeableRegisters().bc.high, 1);
	}},
	{4, [](Z80Cpu& cpu)
	{
		/* 05 DEC B */
		cpu.SubtractByteRegister(cpu.CurrentExchangeableRegisters().bc.high, 1);
	}},
	{7, [](Z80Cpu& cpu)
	{
		/* 06 LD B, n */
		cpu.LoadByteConst(cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 07 RLCA */
		cpu.RotateLeftCircular(cpu.CurrentAf().high);
	}},
	{4, [](Z80Cpu& cpu)
	{
		/* 08 EX AF, AF' */
		cpu.afIndex = 1 - cpu.afIndex;
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* 09 ADD HL, BC */
		cpu.AddWord(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{7, [](Z80Cpu& cpu)
	{
		/* 0a LD A, (BC) */
		cpu.LoadByte(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.word);
	} },
	{6, [](Z80Cpu& cpu)
	{
		/* 0b DEC BC */
		cpu.AddWordOffset(cpu.CurrentExchangeableRegisters().bc.word, -1);
	} },
	{4, [](Z80Cpu& cpu)
	{
		/* 0c INC C */
		cpu.AddByteRegister(cpu.CurrentExchangeableRegisters().bc.low, 1);
	} },
	{4, [](Z80Cpu& cpu)
	{
		/* 0d DEC C */
		cpu.SubtractByteRegister(cpu.CurrentExchangeableRegisters().bc.low, 1);
	} },
	{7, [](Z80Cpu& cpu)
		{
		/* 0e LD C, n */
		cpu.LoadByteConst(cpu.CurrentExchangeableRegisters().bc.low);
	}
},
{4, [](Z80Cpu& cpu)
{
		/* 0f RRCA */
		cpu.RotateRightCircular(cpu.CurrentAf().high);
	} },
	{ 8 /* or 13 */, [](Z80Cpu& cpu)
	{
		/* 10 DJNZ e */
		cpu.DecrementJump(cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 10, [](Z80Cpu& cpu)
	{
		/* 11 LD DE, nn */
		cpu.LoadConstWord(cpu.CurrentExchangeableRegisters().de.word);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 12 LD (DE), A */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().de.word, cpu.CurrentAf().high);
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 13 INC DE */
		cpu.AddWordOffset(cpu.CurrentExchangeableRegisters().de.word, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 14 INC D */
		cpu.AddByteRegister(cpu.CurrentExchangeableRegisters().de.high, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 15 DEC D */
		cpu.SubtractByteRegister(cpu.CurrentExchangeableRegisters().de.high, 1);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 16 LD D, n */
		cpu.LoadByteConst(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 17 RLA */
		cpu.RotateLeft(cpu.CurrentAf().high);
	} },
	{ 12, [](Z80Cpu& cpu)
	{
		/* 18 JR e */
		cpu.JumpRelative();
	} },
	{ 11, [](Z80Cpu& cpu)
	{
		/* 19 ADD HL, DE */
		cpu.AddWord(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().de.word);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 1a LD A, (DE) */
		cpu.LoadByte(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.word);
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 1b DEC DE */
		cpu.AddWordOffset(cpu.CurrentExchangeableRegisters().de.word, -1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 1c INC E */
		cpu.AddByteRegister(cpu.CurrentExchangeableRegisters().de.low, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 1d DEC E */
		cpu.SubtractByteRegister(cpu.CurrentExchangeableRegisters().de.low, 1);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 1e LD E, n */
		cpu.LoadByteConst(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 1f RRA */
		cpu.RotateRight(cpu.CurrentAf().high);
	} },
	{ 7 /* or 12*/,[](Z80Cpu& cpu)
	{
		/* 20 JR NZ, e */
		cpu.JumpRelative([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) == 0;
		});
	}},
	{ 10, [](Z80Cpu& cpu)
	{
		/* 21 LD HL, nn */
		cpu.LoadConstWord(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 16, [](Z80Cpu& cpu)
	{
		/* 22 LD (nn), HL */
		cpu.StoreWordToMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 23 INC HL */
		cpu.AddWordOffset(cpu.CurrentExchangeableRegisters().hl.word, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 24 INC H */
		cpu.AddByteRegister(cpu.CurrentExchangeableRegisters().hl.high, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 25 DEC H */
		cpu.SubtractByteRegister(cpu.CurrentExchangeableRegisters().hl.high, 1);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 26 LD H, n */
		cpu.LoadByteConst(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 27 DAA */
		cpu.DecimalAdjust(cpu.CurrentAf().high);
	} },
	{ 7 /* or 12*/, [](Z80Cpu& cpu)
	{
		/* 28 JR Z, e */
		cpu.JumpRelative([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) != 0;
		});
	} },
	{ 11, [](Z80Cpu& cpu)
	{
		/* 29 ADD HL, HL */
		cpu.AddWord(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 16, [](Z80Cpu& cpu)
	{
		/* 2a LD HL, (nn) */
		cpu.LoadWordFromMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 2b DEC HL */
		cpu.AddWordOffset(cpu.CurrentExchangeableRegisters().hl.word, -1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 2c INC L */
		cpu.AddByteRegister(cpu.CurrentExchangeableRegisters().hl.low, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 2d DEC L */
		cpu.SubtractByteRegister(cpu.CurrentExchangeableRegisters().hl.low, 1);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 2e LD L, n */
		cpu.LoadByteConst(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 2f CPL */
		cpu.Complement(cpu.CurrentAf().high);
	} },
	{ 7 /* or 12*/, [](Z80Cpu& cpu)
	{
		/* 30 JR NC, e */
		cpu.JumpRelative([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) == 0;
		});
	} },
	{ 10, [](Z80Cpu& cpu)
	{
		/* 31 LD SP, nn */
		cpu.LoadConstWord(cpu.sp);
	} },
	{ 13, [](Z80Cpu& cpu)
	{
		/* 32 LD (nn), A */
		cpu.StoreByteToMemory(cpu.CurrentAf().high);
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 33 INC SP */
		cpu.AddWordOffset(cpu.sp, 1);
	} },
	{ 11, [](Z80Cpu& cpu)
	{
		/* 34 INC (HL) */
		cpu.AddByteMemory(cpu.CurrentExchangeableRegisters().hl.word, 1);
	} },
	{ 11, [](Z80Cpu& cpu)
	{
		/* 35 DEC (HL) */
		cpu.SubtractByteMemory(cpu.CurrentExchangeableRegisters().hl.word, 1);
	} },
	{ 10, [](Z80Cpu& cpu)
	{
		/* 36 LD (HL), n */
		cpu.StoreByteConst(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 37 SCF */
		cpu.SetFlag(Flag::Carry);
		cpu.ClearFlag(Flag::AddSubtract | Flag::HalfCarry);
	} },
	{ 7 /* or 12*/, [](Z80Cpu& cpu)
	{
		/* 38 JR C, e */
		cpu.JumpRelative([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) != 0;
		});
	} },
	{ 11, [](Z80Cpu& cpu)
	{
		/* 39 ADD HL, SP */
		cpu.AddWord(cpu.CurrentExchangeableRegisters().hl.word, cpu.sp);
	} },
	{ 13, [](Z80Cpu& cpu)
	{
		/* 3a LD A, (nn) */
		cpu.LoadByte(cpu.CurrentAf().high, cpu.FetchWord());
	} },
	{ 6, [](Z80Cpu& cpu)
	{
		/* 3b DEC SP */
		cpu.AddWordOffset(cpu.sp, -1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 3c INC A */
		cpu.AddByteRegister(cpu.CurrentAf().high, 1);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 3d DEC A */
		cpu.AddByteRegister(cpu.CurrentAf().high, -1);
	} },
	{ 7, [](Z80Cpu& cpu)
	{
		/* 3e LD A, n */
		cpu.LoadByteConst(cpu.CurrentAf().high);
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* 3f CCF */
		if ((cpu.CurrentAf().low & Flag::Carry) != 0) {
			cpu.ClearFlag(Flag::Carry);
		}
		else {
			cpu.SetFlag(Flag::Carry);
		}
		cpu.ClearFlag(Flag::AddSubtract | Flag::HalfCarry);
	} },
	{ 4,[](Z80Cpu& cpu)
	{
		/* 40 LD B, B */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentExchangeableRegisters().bc.high;
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 41 LD B, C */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 42 LD B, D */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 43 LD B, E */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 44 LD B, H */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 45 LD B, L */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentExchangeableRegisters().hl.low;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 46 LD B, (HL) */
		cpu.LoadByte(cpu.CurrentExchangeableRegisters().bc.high, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 47 LD B, A */
		cpu.CurrentExchangeableRegisters().bc.high = cpu.CurrentAf().high;
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 48 LD C, B */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentExchangeableRegisters().bc.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 49 LD C, C */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 4a LD C, D */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 4b LD C, E */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 4c LD C, H */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 4d LD C, L */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentExchangeableRegisters().hl.low;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 4e LD C, (HL) */
		cpu.LoadByte(cpu.CurrentExchangeableRegisters().bc.low, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 4f LD C, A */
		cpu.CurrentExchangeableRegisters().bc.low = cpu.CurrentAf().high;
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 50 LD D, B */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentExchangeableRegisters().bc.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 51 LD D, C */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 52 LD D, D */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 53 LD D, E */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 54 LD D, H */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 55 LD D, L */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentExchangeableRegisters().hl.low;
	} },
	{7,[](Z80Cpu& cpu)
	{
		/* 56 LD D, (HL) */
		cpu.LoadByte(cpu.CurrentExchangeableRegisters().de.high, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 57 LD D, A */
		cpu.CurrentExchangeableRegisters().de.high = cpu.CurrentAf().high;
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 58 LD E, B */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentExchangeableRegisters().bc.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 59 LD E, C */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 5a LD E, D */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 5b LD E, E */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 5c LD E, H */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 5d LD E, L */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentExchangeableRegisters().hl.low;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 5e LD E, (HL) */
		cpu.LoadByte(cpu.CurrentExchangeableRegisters().de.low, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 5f LD E, A */
		cpu.CurrentExchangeableRegisters().de.low = cpu.CurrentAf().high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 60 LD H, B */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentExchangeableRegisters().bc.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 61 LD H, C */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 62 LD H, D */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 63 LD H, E */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 64 LD H, H */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 65 LD H, L */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentExchangeableRegisters().hl.low;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 66 LD H, (HL) */
		cpu.LoadByte(cpu.CurrentExchangeableRegisters().hl.high, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 67 LD H, A */
		cpu.CurrentExchangeableRegisters().hl.high = cpu.CurrentAf().high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 68 LD L, B */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentExchangeableRegisters().bc.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 69 LD L, C */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 6a LD L, D */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 6b LD L, E */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 6c LD L, H */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 6d LD L, L */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentExchangeableRegisters().hl.low;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 6e LD L, (HL) */
		cpu.LoadByte(cpu.CurrentExchangeableRegisters().hl.low, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 6f LD L, A */
		cpu.CurrentExchangeableRegisters().hl.low = cpu.CurrentAf().high;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 70 LD (HL), B */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 71 LD (HL), C */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 72 LD (HL), D */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 73 LD (HL), E */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 74 LD (HL), H */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 75 LD (HL), L */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 76 HALT */
		cpu.halted = true;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 77 LD (HL), A */
		cpu.pMemorySpace->Write(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 78 LD A, B */
		cpu.CurrentAf().high = cpu.CurrentExchangeableRegisters().bc.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 79 LD A, C */
		cpu.CurrentAf().high = cpu.CurrentExchangeableRegisters().bc.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 7a LD A, D */
		cpu.CurrentAf().high = cpu.CurrentExchangeableRegisters().de.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 7b LD A, E */
		cpu.CurrentAf().high = cpu.CurrentExchangeableRegisters().de.low;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 7c LD A, H */
		cpu.CurrentAf().high = cpu.CurrentExchangeableRegisters().hl.high;
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 7d LD A, L */
		cpu.CurrentAf().high = cpu.CurrentExchangeableRegisters().hl.low;
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 7e LD A, (HL) */
		cpu.LoadByte(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 7f LD A, A */
		cpu.CurrentAf().high = cpu.CurrentAf().high;
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 80 ADD A, B */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 81 ADD A, C */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 82 ADD A, D */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 83 ADD A, E */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 84 ADD A, H */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 85 ADD A, L */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 86 ADD A, (HL) */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 87 ADD A, A */
		cpu.AddByteRegister(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 88 ADC A, B */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 89 ADC A, C */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 8a ADC A, D */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 8b ADC A, E */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 8c ADC A, H */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 8d ADC A, L */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 8e ADC A, (HL) */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 8f ADC A, A */
		cpu.AddByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{ 4,[](Z80Cpu& cpu)
	{
		/* 90 SUB A, B */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 91 SUB A, C */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 92 SUB A, D */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 93 SUB A, E */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 94 SUB A, H */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 95 SUB A, L */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 96 SUB A, (HL) */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 97 SUB A, A */
		cpu.SubtractByteRegister(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* 98 SBC A, B */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 99 SBC A, C */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 9a SBC A, D */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 9b SBC A, E */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 9c SBC A, H */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* 9d SBC A, L */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* 9e SBC A, (HL) */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* 9f SBC A, A */
		cpu.SubtractByteRegisterCarry(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* a0 AND A, B */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* a1 AND A, C */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* a2 AND A, D */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* a3 AND A, E */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* a4 AND A, H */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* a5 AND A, L */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* a6 AND A, (HL) */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* a7 AND A, A */
		cpu.AndByteRegister(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* a8 XOR A, B */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* a9 XOR A, C */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* aa XOR A, D */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* ab XOR A, E */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* ac XOR A, H */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* ad XOR A, L */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* ae XOR A, (HL) */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* af XOR A, A */
		cpu.XorByteRegister(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* b0 OR A, B */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* b1 OR A, C */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* b2 OR A, D */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* b3 OR A, E */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* b4 OR A, H */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* b5 OR A, L */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* b6 OR A, (HL) */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* b7 OR A, A */
		cpu.OrByteRegister(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{4,[](Z80Cpu& cpu)
	{
		/* b8 CP A, B */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* b9 CP A, C */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* ba CP A, D */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* bb CP A, E */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* bc CP A, H */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{4,[](Z80Cpu& cpu)
	{
		/* bd CP A, L */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentExchangeableRegisters().hl.low);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* be CP A, (HL) */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.pMemorySpace->Read(cpu.CurrentExchangeableRegisters().hl.word));
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* bf CP A, A */
		cpu.CompareByteRegister(cpu.CurrentAf().high, cpu.CurrentAf().high);
	} },
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* c0 RET NZ */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) == 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* c1 POP BC */
		cpu.CurrentExchangeableRegisters().bc.word = cpu.PopWord();
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* c2 JP NZ, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) == 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* c3 JP nn */
		cpu.JumpAbsolute([](uint8_t)
		{
			return true;
		});
	} },
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* c4 CALL NZ, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) == 0;
		});
	}},
	{ 11, [](Z80Cpu& cpu)
	{
		/* c5 PUSH BC */
		cpu.PushWord(cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* c6 ADD A, n */
		cpu.AddByteConst(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* c7 RST 00h */
		cpu.Call(0x0000);
	}},
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* c8 RET Z */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) != 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* c9 RET */
		cpu.Return();
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* ca JP Z, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) != 0;
		});
	}},
	{0,[](Z80Cpu& cpu)
	{
		/* cb PREFIX CB */
		assert(false); // Should never be called.
	} },
	{ 10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* cc CALL Z, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Zero) != 0;
		});
	}},
	{ 17, [](Z80Cpu& cpu)
	{
		/* cd CALL nn */
		cpu.Call();
	} },
	{7,[](Z80Cpu& cpu)
	{
		/* ce ADC A, n */
		cpu.AddByteConstCarry(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* cf RST 08h */
		cpu.Call(0x0008);
	} },
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* d0 RET NC */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) == 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* d1 POP DE */
		cpu.CurrentExchangeableRegisters().de.word = cpu.PopWord();
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* d2 JP NC, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) == 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* d3 OUT (n), A */
		cpu.WriteIo(cpu.CurrentAf().high);
	} },
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* d4 CALL NC, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) == 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* d5 PUSH DE */
		cpu.PushWord(cpu.CurrentExchangeableRegisters().de.word);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* d6 SUB A, n */
		cpu.SubtractByteConst(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* d7 RST 10h */
		cpu.Call(0x0010);
	} },
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* d8 RET C */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) != 0;
		});
	}},
	{ 4,[](Z80Cpu& cpu)
	{
		/* d9 EXX */
		cpu.exchangeableRegisterIndex = 1 - cpu.exchangeableRegisterIndex;
	} },
	{10,[](Z80Cpu& cpu)
	{
		/* da JP C, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) != 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* db IN A, (n) */
		cpu.ReadIo(cpu.CurrentAf().high);
	} },
	{ 10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* dc CALL C, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Carry) != 0;
		});
	} },
	{ 17, [](Z80Cpu& cpu)
	{
		/* dd PREFIX DD */
		assert(false); // Should never be called.
	} },
	{7,[](Z80Cpu& cpu)
	{
		/* de SUB A, n */
		cpu.SubtractByteConst(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* df RST 18h */
		cpu.Call(0x0018);
	} },
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* e0 RET PO */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::ParityOverflow) == 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* e1 POP HL */
		cpu.CurrentExchangeableRegisters().hl.word = cpu.PopWord();
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* e2 JP PO, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::ParityOverflow) == 0;
		});
	}},
	{19,[](Z80Cpu& cpu)
	{
		/* e3 EX (SP), HL */
		cpu.ExchangeStack(cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* e4 CALL PO, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::ParityOverflow) == 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* e5 PUSH HL */
		cpu.PushWord(cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* e6 AND A, n */
		cpu.AndByteConst(cpu.CurrentAf().high);
	}},
	{ 12,[](Z80Cpu& cpu)
	{
		/* e7 RST 20h */
		cpu.Call(0x0020);
	} },
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* e8 RET PE */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::ParityOverflow) != 0;
		});
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* e9 JP (HL) */
		cpu.JumpAbsolute(cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* ea JP PE, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::ParityOverflow) != 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* eb EX DE, HL */
		cpu.ExchangeRegisters(cpu.CurrentExchangeableRegisters().de.word, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* ec CALL PE, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::ParityOverflow) != 0;
		});
	}},
	{17, [](Z80Cpu& cpu)
	{
		/* ed PREFIX ED */
		assert(false); // Should never be called.
	} },
	{7,[](Z80Cpu& cpu)
	{
		/* ee XOR A, n */
		cpu.XorByteConst(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* ef RST 28h */
		cpu.Call(0x0028);
	} },
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* f0 RET P */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Sign) == 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* f1 POP AF */
		cpu.CurrentAf().word = cpu.PopWord();
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* f2 JP P, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Sign) == 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* f3 DI */
		cpu.iffs[0] = 0x01;
	}},
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* f4 CALL P, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Sign) == 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* f5 PUSH AF */
		cpu.PushWord(cpu.CurrentAf().word);
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* f6 OR A, n */
		cpu.OrByteConst(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* f7 RST 30h */
		cpu.Call(0x0030);
	}},
	{5 /* or 11 */,[](Z80Cpu& cpu)
	{
		/* f8 RET M */
		cpu.Return([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Sign) != 0;
		});
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* LD SP, HL */
		cpu.sp = cpu.CurrentExchangeableRegisters().hl.word;
	}},
	{10,[](Z80Cpu& cpu)
	{
		/* fa JP M, nn */
		cpu.JumpAbsolute([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Sign) != 0;
		});
	}},
	{11,[](Z80Cpu& cpu)
	{
		/* fb EI */
		cpu.iffs[0] = 0x00;
	}},
	{10 /* or 17 */,[](Z80Cpu& cpu)
	{
		/* fc CALL M, nn */
		cpu.Call([&cpu](uint8_t)
		{
			return (cpu.CurrentAf().low & Flag::Sign) != 0;
		});
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* fd PREFIX FD */
		assert(false); // Should never be called.
	}},
	{7,[](Z80Cpu& cpu)
	{
		/* fe CP A, n */
		cpu.CompareByteConst(cpu.CurrentAf().high);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* ff RST 38h */
		cpu.Call(0x0038);
	} },
};
const Z80Cpu::Instruction Z80Cpu::InstructionsAfterCb[] = {
	{  8, [](Z80Cpu& cpu)
	{
		/* 00 RLC B */
		cpu.RotateLeftCircular(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 01 RLC C */
		cpu.RotateLeftCircular(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 02 RLC D */
		cpu.RotateLeftCircular(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 03 RLC E */
		cpu.RotateLeftCircular(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 04 RLC H */
		cpu.RotateLeftCircular(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 05 RLC L */
		cpu.RotateLeftCircular(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 06 RLC (HL) */
		cpu.RotateLeftCircularMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 07 RLC A */
		cpu.RotateLeftCircular(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 08 RRC B */
		cpu.RotateRightCircular(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 09 RRC C */
		cpu.RotateRightCircular(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 0a RRC D */
		cpu.RotateRightCircular(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 0b RRC E */
		cpu.RotateRightCircular(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 0c RRC H */
		cpu.RotateRightCircular(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 0d RRC L */
		cpu.RotateRightCircular(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 0e RRC (HL) */
		cpu.RotateRightCircularMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 0f RRC A */
		cpu.RotateRightCircular(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 10 RL B */
		cpu.RotateLeft(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 11 RL C */
		cpu.RotateLeft(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 12 RL D */
		cpu.RotateLeft(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 13 RL E */
		cpu.RotateLeft(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 14 RL H */
		cpu.RotateLeft(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 15 RL L */
		cpu.RotateLeft(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 16 RL (HL) */
		cpu.RotateLeftMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 17 RL A */
		cpu.RotateLeft(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 18 RR B */
		cpu.RotateRight(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 19 RR C */
		cpu.RotateRight(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 1a RR D */
		cpu.RotateRight(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 1b RR E */
		cpu.RotateRight(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 1c RR H */
		cpu.RotateRight(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 1d RR L */
		cpu.RotateRight(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 1e RR (HL) */
		cpu.RotateRightMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 1f RR A */
		cpu.RotateRight(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 20 SLA B */
		cpu.ShiftLeftArithmetic(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 21 SLA C */
		cpu.ShiftLeftArithmetic(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 22 SLA D */
		cpu.ShiftLeftArithmetic(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 23 SLA E */
		cpu.ShiftLeftArithmetic(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 24 SLA H */
		cpu.ShiftLeftArithmetic(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 25 SLA L */
		cpu.ShiftLeftArithmetic(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 26 SLA (HL) */
		cpu.ShiftLeftArithmeticMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 27 SLA A */
		cpu.ShiftLeftArithmetic(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 28 SRA B */
		cpu.ShiftRightArithmetic(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 29 SRA C */
		cpu.ShiftRightArithmetic(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 2a SRA D */
		cpu.ShiftRightArithmetic(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 2b SRA E */
		cpu.ShiftRightArithmetic(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 2c SRA H */
		cpu.ShiftRightArithmetic(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 2d SRA L */
		cpu.ShiftRightArithmetic(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 2e SRA (HL) */
		cpu.ShiftRightArithmeticMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* 2f SRA A */
		cpu.ShiftRightArithmetic(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 30 SLL B */
		cpu.ShiftLeftLogical(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 31 SLL C */
		cpu.ShiftLeftLogical(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 32 SLL D */
		cpu.ShiftLeftLogical(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 33 SLL E */
		cpu.ShiftLeftLogical(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 34 SLL H */
		cpu.ShiftLeftLogical(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 35 SLL L */
		cpu.ShiftLeftLogical(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 36 SLL (HL) */
		cpu.ShiftLeftLogicalMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* 37 SLL A */
		cpu.ShiftLeftLogical(cpu.CurrentAf().high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 38 SRL B */
		cpu.ShiftRightLogical(cpu.CurrentExchangeableRegisters().bc.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 39 SRL C */
		cpu.ShiftRightLogical(cpu.CurrentExchangeableRegisters().bc.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 3a SRL D */
		cpu.ShiftRightLogical(cpu.CurrentExchangeableRegisters().de.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 3b SRL E */
		cpu.ShiftRightLogical(cpu.CurrentExchangeableRegisters().de.low);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 3c SRL H */
		cpu.ShiftRightLogical(cpu.CurrentExchangeableRegisters().hl.high);
	} },
	{  8, [](Z80Cpu& cpu)
	{
		/* 3d SRL L */
		cpu.ShiftRightLogical(cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* 3e SRL (HL) */
		cpu.ShiftRightLogicalMemory(cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* 3f SRL A */
		cpu.ShiftRightLogical(cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 40 BIT 0, B */
		cpu.TestBit(0, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 41 BIT 0, C */
		cpu.TestBit(0, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 42 BIT 0, D */
		cpu.TestBit(0, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 43 BIT 0, E */
		cpu.TestBit(0, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 44 BIT 0, H */
		cpu.TestBit(0, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 45 BIT 0, L */
		cpu.TestBit(0, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 46 BIT 0, (HL) */
		cpu.TestBitMemory(0, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 47 BIT 0, A */
		cpu.TestBit(0, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 48 BIT 1, B */
		cpu.TestBit(1, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 49 BIT 1, C */
		cpu.TestBit(1, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 4a BIT 1, D */
		cpu.TestBit(1, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 4b BIT 1, E */
		cpu.TestBit(1, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 4c BIT 1, H */
		cpu.TestBit(1, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 4d BIT 1, L */
		cpu.TestBit(1, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 4e BIT 1, (HL) */
		cpu.TestBitMemory(1, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 4f BIT 1, A */
		cpu.TestBit(1, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 50 BIT 2, B */
		cpu.TestBit(2, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 51 BIT 2, C */
		cpu.TestBit(2, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 52 BIT 2, D */
		cpu.TestBit(2, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 53 BIT 2, E */
		cpu.TestBit(2, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 54 BIT 2, H */
		cpu.TestBit(2, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 55 BIT 2, L */
		cpu.TestBit(2, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 56 BIT 2, (HL) */
		cpu.TestBitMemory(2, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 57 BIT 2, A */
		cpu.TestBit(2, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 58 BIT 3, B */
		cpu.TestBit(3, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 59 BIT 3, C */
		cpu.TestBit(3, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 5a BIT 3, D */
		cpu.TestBit(3, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 5b BIT 3, E */
		cpu.TestBit(3, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 5c BIT 3, H */
		cpu.TestBit(3, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 5d BIT 3, L */
		cpu.TestBit(3, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 5e BIT 3, (HL) */
		cpu.TestBitMemory(3, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 5f BIT 3, A */
		cpu.TestBit(3, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 60 BIT 4, B */
		cpu.TestBit(4, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 61 BIT 4, C */
		cpu.TestBit(4, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 62 BIT 4, D */
		cpu.TestBit(4, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 63 BIT 4, E */
		cpu.TestBit(4, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 64 BIT 4, H */
		cpu.TestBit(4, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 65 BIT 4, L */
		cpu.TestBit(4, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 66 BIT 4, (HL) */
		cpu.TestBitMemory(4, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 67 BIT 4, A */
		cpu.TestBit(4, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 68 BIT 5, B */
		cpu.TestBit(5, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 69 BIT 5, C */
		cpu.TestBit(5, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 6a BIT 5, D */
		cpu.TestBit(5, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 6b BIT 5, E */
		cpu.TestBit(5, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 6c BIT 5, H */
		cpu.TestBit(5, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 6d BIT 5, L */
		cpu.TestBit(5, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 6e BIT 5, (HL) */
		cpu.TestBitMemory(5, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 6f BIT 5, A */
		cpu.TestBit(5, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 70 BIT 6, B */
		cpu.TestBit(6, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 71 BIT 6, C */
		cpu.TestBit(6, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 72 BIT 6, D */
		cpu.TestBit(6, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 73 BIT 6, E */
		cpu.TestBit(6, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 74 BIT 6, H */
		cpu.TestBit(6, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 75 BIT 6, L */
		cpu.TestBit(6, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 76 BIT 6, (HL) */
		cpu.TestBitMemory(6, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 77 BIT 6, A */
		cpu.TestBit(6, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 78 BIT 7, B */
		cpu.TestBit(7, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 79 BIT 7, C */
		cpu.TestBit(7, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 7a BIT 7, D */
		cpu.TestBit(7, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 7b BIT 7, E */
		cpu.TestBit(7, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 7c BIT 7, H */
		cpu.TestBit(7, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 7d BIT 7, L */
		cpu.TestBit(7, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 7e BIT 7, (HL) */
		cpu.TestBitMemory(7, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 7f BIT 7, A */
		cpu.TestBit(7, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 80 RES 0, B */
		cpu.ResetBit(0, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 81 RES 0, C */
		cpu.ResetBit(0, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 82 RES 0, D */
		cpu.ResetBit(0, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 83 RES 0, E */
		cpu.ResetBit(0, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 84 RES 0, H */
		cpu.ResetBit(0, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 85 RES 0, L */
		cpu.ResetBit(0, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* 86 RES 0, (HL) */
		cpu.ResetBitMemory(0, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 87 RES 0, A */
		cpu.ResetBit(0, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 88 RES 1, B */
		cpu.ResetBit(1, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 89 RES 1, C */
		cpu.ResetBit(1, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 8a RES 1, D */
		cpu.ResetBit(1, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 8b RES 1, E */
		cpu.ResetBit(1, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 8c RES 1, H */
		cpu.ResetBit(1, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 8d RES 1, L */
		cpu.ResetBit(1, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* 8e RES 1, (HL) */
		cpu.ResetBitMemory(1, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 8f RES 1, A */
		cpu.ResetBit(1, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 90 RES 2, B */
		cpu.ResetBit(2, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 91 RES 2, C */
		cpu.ResetBit(2, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 92 RES 2, D */
		cpu.ResetBit(2, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 93 RES 2, E */
		cpu.ResetBit(2, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 94 RES 2, H */
		cpu.ResetBit(2, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 95 RES 2, L */
		cpu.ResetBit(2, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* 96 RES 2, (HL) */
		cpu.ResetBitMemory(2, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 97 RES 2, A */
		cpu.ResetBit(2, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* 98 RES 3, B */
		cpu.ResetBit(3, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 99 RES 3, C */
		cpu.ResetBit(3, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 9a RES 3, D */
		cpu.ResetBit(3, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 9b RES 3, E */
		cpu.ResetBit(3, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 9c RES 3, H */
		cpu.ResetBit(3, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 9d RES 3, L */
		cpu.ResetBit(3, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* 9e RES 3, (HL) */
		cpu.ResetBitMemory(3, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 9f RES 3, A */
		cpu.ResetBit(3, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* a0 RES 4, B */
		cpu.ResetBit(4, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a1 RES 4, C */
		cpu.ResetBit(4, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a2 RES 4, D */
		cpu.ResetBit(4, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a3 RES 4, E */
		cpu.ResetBit(4, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a4 RES 4, H */
		cpu.ResetBit(4, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a5 RES 4, L */
		cpu.ResetBit(4, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* a6 RES 4, (HL) */
		cpu.ResetBitMemory(4, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a7 RES 4, A */
		cpu.ResetBit(4, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* a8 RES 5, B */
		cpu.ResetBit(5, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* a9 RES 5, C */
		cpu.ResetBit(5, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* aa RES 5, D */
		cpu.ResetBit(5, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* ab RES 5, E */
		cpu.ResetBit(5, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* ac RES 5, H */
		cpu.ResetBit(5, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* ad RES 5, L */
		cpu.ResetBit(5, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* ae RES 5, (HL) */
		cpu.ResetBitMemory(5, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* af RES 5, A */
		cpu.ResetBit(5, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* b0 RES 6, B */
		cpu.ResetBit(6, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b1 RES 6, C */
		cpu.ResetBit(6, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b2 RES 6, D */
		cpu.ResetBit(6, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b3 RES 6, E */
		cpu.ResetBit(6, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b4 RES 6, H */
		cpu.ResetBit(6, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b5 RES 6, L */
		cpu.ResetBit(6, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* b6 RES 6, (HL) */
		cpu.ResetBitMemory(6, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b7 RES 6, A */
		cpu.ResetBit(6, cpu.CurrentAf().high);
	} },
	{8,[](Z80Cpu& cpu)
	{
		/* b8 RES 7, B */
		cpu.ResetBit(7, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* b9 RES 7, C */
		cpu.ResetBit(7, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* ba RES 7, D */
		cpu.ResetBit(7, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* bb RES 7, E */
		cpu.ResetBit(7, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* bc RES 7, H */
		cpu.ResetBit(7, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* bd RES 7, L */
		cpu.ResetBit(7, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15,[](Z80Cpu& cpu)
	{
		/* be RES 7, (HL) */
		cpu.ResetBitMemory(7, cpu.CurrentExchangeableRegisters().hl.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* bf RES 7, A */
		cpu.ResetBit(7, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* c0 SET 0, B */
		cpu.SetBit(0, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* c1 SET 0, C */
		cpu.SetBit(0, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* c2 SET 0, D */
		cpu.SetBit(0, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* c3 SET 0, E */
		cpu.SetBit(0, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* c4 SET 0, H */
		cpu.SetBit(0, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* c5 SET 0, L */
		cpu.SetBit(0, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* c6 SET 0, (HL) */
		cpu.SetBitMemory(0, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* c7 SET 0, A */
		cpu.SetBit(0, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* c8 SET 1, B */
		cpu.SetBit(1, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* c9 SET 1, C */
		cpu.SetBit(1, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* ca SET 1, D */
		cpu.SetBit(1, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* cb SET 1, E */
		cpu.SetBit(1, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* cc SET 1, H */
		cpu.SetBit(1, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* cd SET 1, L */
		cpu.SetBit(1, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* ce SET 1, (HL) */
		cpu.SetBitMemory(1, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* cf SET 1, A */
		cpu.SetBit(1, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* d0 SET 2, B */
		cpu.SetBit(2, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* d1 SET 2, C */
		cpu.SetBit(2, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* d2 SET 2, D */
		cpu.SetBit(2, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* d3 SET 2, E */
		cpu.SetBit(2, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* d4 SET 2, H */
		cpu.SetBit(2, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* d5 SET 2, L */
		cpu.SetBit(2, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* d6 SET 2, (HL) */
		cpu.SetBitMemory(2, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* d7 SET 2, A */
		cpu.SetBit(2, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* d8 SET 3, B */
		cpu.SetBit(3, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* d9 SET 3, C */
		cpu.SetBit(3, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* da SET 3, D */
		cpu.SetBit(3, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* db SET 3, E */
		cpu.SetBit(3, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* dc SET 3, H */
		cpu.SetBit(3, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* dd SET 3, L */
		cpu.SetBit(3, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* de SET 3, (HL) */
		cpu.SetBitMemory(3, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* df SET 3, A */
		cpu.SetBit(3, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* e0 SET 4, B */
		cpu.SetBit(4, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* e1 SET 4, C */
		cpu.SetBit(4, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* e2 SET 4, D */
		cpu.SetBit(4, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* e3 SET 4, E */
		cpu.SetBit(4, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* e4 SET 4, H */
		cpu.SetBit(4, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* e5 SET 4, L */
		cpu.SetBit(4, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* e6 SET 4, (HL) */
		cpu.SetBitMemory(4, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* e7 SET 4, A */
		cpu.SetBit(4, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* e8 SET 5, B */
		cpu.SetBit(5, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* e9 SET 5, C */
		cpu.SetBit(5, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* ea SET 5, D */
		cpu.SetBit(5, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* eb SET 5, E */
		cpu.SetBit(5, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* ec SET 5, H */
		cpu.SetBit(5, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* ed SET 5, L */
		cpu.SetBit(5, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* ee SET 5, (HL) */
		cpu.SetBitMemory(5, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* ef SET 5, A */
		cpu.SetBit(5, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* f0 SET 6, B */
		cpu.SetBit(6, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* f1 SET 6, C */
		cpu.SetBit(6, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* f2 SET 6, D */
		cpu.SetBit(6, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* f3 SET 6, E */
		cpu.SetBit(6, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* f4 SET 6, H */
		cpu.SetBit(6, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* f5 SET 6, L */
		cpu.SetBit(6, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* f6 SET 6, (HL) */
		cpu.SetBitMemory(6, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* f7 SET 6, A */
		cpu.SetBit(6, cpu.CurrentAf().high);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* f8 SET 7, B */
		cpu.SetBit(7, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* f9 SET 7, C */
		cpu.SetBit(7, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* fa SET 7, D */
		cpu.SetBit(7, cpu.CurrentExchangeableRegisters().de.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* fb SET 7, E */
		cpu.SetBit(7, cpu.CurrentExchangeableRegisters().de.low);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* fc SET 7, H */
		cpu.SetBit(7, cpu.CurrentExchangeableRegisters().hl.high);
	}},
	{ 8, [](Z80Cpu& cpu)
	{
		/* fd SET 7, L */
		cpu.SetBit(7, cpu.CurrentExchangeableRegisters().hl.low);
	} },
	{15, [](Z80Cpu& cpu)
	{
		/* fe SET 7, (HL) */
		cpu.SetBitMemory(7, cpu.CurrentExchangeableRegisters().hl.word);
	} },
	{ 8, [](Z80Cpu& cpu)
	{
		/* ff SET 7, A */
		cpu.SetBit(7, cpu.CurrentAf().high);
	} },
};
const Z80Cpu::Instruction Z80Cpu::InstructionsAfterEd[] = {
	{ 8, Nop /* 00 */},
	{ 8, Nop /* 01 */},
	{ 8, Nop /* 02 */},
	{ 8, Nop /* 03 */},
	{ 8, Nop /* 04 */},
	{ 8, Nop /* 05 */},
	{ 8, Nop /* 06 */},
	{ 8, Nop /* 07 */},
	{ 8, Nop /* 08 */},
	{ 8, Nop /* 09 */},
	{ 8, Nop /* 0a */},
	{ 8, Nop /* 0b */},
	{ 8, Nop /* 0c */},
	{ 8, Nop /* 0d */},
	{ 8, Nop /* 0e */},
	{ 8, Nop /* 0f */},
	{ 8, Nop /* 10 */},
	{ 8, Nop /* 11 */},
	{ 8, Nop /* 12 */},
	{ 8, Nop /* 13 */},
	{ 8, Nop /* 14 */},
	{ 8, Nop /* 15 */},
	{ 8, Nop /* 16 */},
	{ 8, Nop /* 17 */},
	{ 8, Nop /* 18 */},
	{ 8, Nop /* 19 */},
	{ 8, Nop /* 1a */},
	{ 8, Nop /* 1b */},
	{ 8, Nop /* 1c */},
	{ 8, Nop /* 1d */},
	{ 8, Nop /* 1e */},
	{ 8, Nop /* 1f */},
	{ 8, Nop /* 20 */},
	{ 8, Nop /* 21 */},
	{ 8, Nop /* 22 */},
	{ 8, Nop /* 23 */},
	{ 8, Nop /* 24 */},
	{ 8, Nop /* 25 */},
	{ 8, Nop /* 26 */},
	{ 8, Nop /* 27 */},
	{ 8, Nop /* 28 */},
	{ 8, Nop /* 29 */},
	{ 8, Nop /* 2a */},
	{ 8, Nop /* 2b */},
	{ 8, Nop /* 2c */},
	{ 8, Nop /* 2d */},
	{ 8, Nop /* 2e */},
	{ 8, Nop /* 2f */},
	{ 8, Nop /* 30 */},
	{ 8, Nop /* 31 */},
	{ 8, Nop /* 32 */},
	{ 8, Nop /* 33 */},
	{ 8, Nop /* 34 */},
	{ 8, Nop /* 35 */},
	{ 8, Nop /* 36 */},
	{ 8, Nop /* 37 */},
	{ 8, Nop /* 38 */},
	{ 8, Nop /* 39 */},
	{ 8, Nop /* 3a */},
	{ 8, Nop /* 3b */},
	{ 8, Nop /* 3c */},
	{ 8, Nop /* 3d */},
	{ 8, Nop /* 3e */},
	{ 8, Nop /* 3f */},
	{12, [](Z80Cpu& cpu)
	{
		/* 40 IN B, (C) */
		cpu.ReadIo(cpu.CurrentExchangeableRegisters().bc.low, cpu.CurrentExchangeableRegisters().bc.word);
	} },
	{12,[](Z80Cpu& cpu)
	{
		/* 41 OUT (C),B */
		cpu.WriteIo(cpu.CurrentExchangeableRegisters().bc.word, cpu.CurrentExchangeableRegisters().bc.high);
	}},
	{15,[](Z80Cpu& cpu)
	{
		/* 42 SBC HL, BC */
		cpu.SubtractWordCarry(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{ 20,[](Z80Cpu& cpu)
	{
		/* 43 LD (nn), BC */
		cpu.StoreWordToMemory(cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{8,[](Z80Cpu& cpu)
	{
		/* 44 NEG */
		cpu.Negate(cpu.CurrentAf().high);
	}},
	{ 8, Nop /* 45 */},
	{ 8, Nop /* 46 */},
	{ 8, Nop /* 47 */},
	{12,[](Z80Cpu& cpu)
	{
		/* 48 IN C, (C) */
		cpu.ReadIo(cpu.CurrentExchangeableRegisters().bc.low, cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{12,[](Z80Cpu& cpu)
	{
		/* 49 OUT (C), C */
		cpu.WriteIo(cpu.CurrentExchangeableRegisters().bc.word, cpu.CurrentExchangeableRegisters().bc.low);
	}},
	{15,[](Z80Cpu& cpu)
	{
		/* 4a ADC HL, BC */
		cpu.AddWordCarry(cpu.CurrentExchangeableRegisters().hl.word, cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{ 20,[](Z80Cpu& cpu)
	{
		/* 4b LD BC, (nn) */
		cpu.LoadWordFromMemory(cpu.CurrentExchangeableRegisters().bc.word);
	}},
	{ 8, Nop /* 4c */ },
	{ 8, Nop /* 4d */},
	{ 8, Nop /* 4e */},
	{ 8, Nop /* 4f */ },
	//{8, [](Z80Cpu& cpu)
	//{
	//	/* LD I, A */
	//	LoadByte(cpu.interrupt, cpu.CurrentAf().high);
	//} },
};
const Z80Cpu::Instruction Z80Cpu::InstructionsAfterDd[] = {
	{8, [](Z80Cpu& cpu)
	{
		/* LD IX, nn */
		cpu.LoadConstWord(cpu.ix.word);
	} },
};
const Z80Cpu::Instruction Z80Cpu::InstructionsAfterDdCb[] = {
	{ 16, [](Z80Cpu& cpu)
	{
		/* RLD (IX+d) */
	} },
};
const Z80Cpu::Instruction Z80Cpu::InstructionsAfterFd[] = {
	{   8, [](Z80Cpu& cpu)
	{
		/* LD IY, nn */
		cpu.LoadConstWord(cpu.iy.word);
	} },
};

uint16_t Z80Cpu::FetchWord()
{
	auto low = FetchByte();
	auto high = FetchByte();
	return MakeWord(high, low);
}

void Z80Cpu::PushWord(uint16_t value)
{
	pMemorySpace->Write(--sp, LowByte(value));
	pMemorySpace->Write(--sp, HighByte(value));
}

uint16_t Z80Cpu::PopWord()
{
	auto high = pMemorySpace->Read(sp++);
	auto low = pMemorySpace->Read(sp++);
	return MakeWord(high, low);
}

void Z80Cpu::FetchInstruction()
{
	if (halted) {
		clockCountToExecute = 1;
		++refresh;
		return;
	}
	uint8_t opcode = FetchByte();
	switch (opcode) {
	case 0xcb:
		pNextInstruction = &InstructionsAfterCb[FetchByte()];
		break;
	case 0xed:
		pNextInstruction = &InstructionsAfterEd[FetchByte()];
		break;
	case 0xdd:
		pNextInstruction = &InstructionsAfterDd[FetchByte()];
		break;
	case 0xfd:
		pNextInstruction = &InstructionsAfterFd[FetchByte()];
		break;
	default:
		pNextInstruction = &Instructions[opcode];
	}
	assert(pNextInstruction->clockCount != 0);
	clockCountToExecute = pNextInstruction->clockCount;
	++refresh;
}

void Z80Cpu::UpdateZeroSignParityFlags(uint8_t value)
{
	if (value == 0) {
		SetFlag(Flag::Zero);
	}
	else {
		ClearFlag(Flag::Zero);
	}
	if (value & 0x80) {
		SetFlag(Flag::Sign);
	}
	else {
		ClearFlag(Flag::Sign);
	}
	int parity = 0;
	for (int i = 0; i < 8; ++i) {
		if (value & (1 << i)) {
			++parity;
		}
	}
	if ((parity & 1) == 0) {
		SetFlag(Flag::ParityOverflow);
	}
	else {
		ClearFlag(Flag::ParityOverflow);
	}
}

void Z80Cpu::LoadConstWord(uint16_t& wordRegister)
{
	wordRegister = FetchWord();
}

void Z80Cpu::LoadByteConst(uint8_t& byteRegister)
{
	byteRegister = FetchByte();
}

void Z80Cpu::LoadByte(uint8_t& byteRegister, uint16_t address)
{
	byteRegister = pMemorySpace->Read(address);
	UpdateZeroSignParityFlags(byteRegister);
}

void Z80Cpu::StoreByteToMemory(uint8_t byteRegister)
{
	auto address = FetchWord();
	pMemorySpace->Write(address, byteRegister);
}

void Z80Cpu::StoreByteConst(uint16_t address)
{
	auto value = FetchByte();
	pMemorySpace->Write(address, value);
}

void Z80Cpu::AddByteConst(uint8_t& byteRegister)
{
	auto value = FetchByte();
	AddByteRegister(byteRegister, value);
}

void Z80Cpu::AddByteRegister(uint8_t& byteRegister, int offset)
{
	byteRegister += offset;
	UpdateZeroSignParityFlags(byteRegister);
	SetFlag(Flag::AddSubtract);
}

void Z80Cpu::AddByteMemory(uint16_t address, int offset)
{
	uint8_t value = pMemorySpace->Read(address);
	value += offset;
	pMemorySpace->Write(address, value);
	UpdateZeroSignParityFlags(value);
	SetFlag(Flag::AddSubtract);
}

void Z80Cpu::AddByteConstCarry(uint8_t& byteRegister)
{
	auto value = FetchByte();
	AddByteRegisterCarry(byteRegister, value);
}

void Z80Cpu::AddByteRegisterCarry(uint8_t& byteRegister, uint8_t offset)
{
	byteRegister += offset + ((CurrentAf().low & Flag::Carry) != 0 ? 1 : 0);
	UpdateZeroSignParityFlags(byteRegister);
	SetFlag(Flag::AddSubtract);
}

void Z80Cpu::SubtractByteConst(uint8_t& byteRegister)
{
	auto value = FetchByte();
	SubtractByteRegister(byteRegister, value);
}

void Z80Cpu::SubtractByteRegister(uint8_t& byteRegister, uint8_t value)
{
	auto result = static_cast<int16_t>(byteRegister) - value;
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	byteRegister = static_cast<uint8_t>(result);
	UpdateZeroSignParityFlags(static_cast<uint8_t>(result));
	ClearFlag(Flag::AddSubtract);
}

void Z80Cpu::SubtractByteMemory(uint16_t address, uint8_t value)
{
	auto memoryValue = pMemorySpace->Read(address);
	auto result = static_cast<int16_t>(memoryValue) - value;
	pMemorySpace->Write(address, static_cast<uint8_t>(result));
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	UpdateZeroSignParityFlags(static_cast<uint8_t>(result));
	ClearFlag(Flag::AddSubtract);
}

void Z80Cpu::SubtractByteRegisterCarry(uint8_t& byteRegister, uint8_t value)
{
	short result = static_cast<int16_t>(byteRegister) - value - ((CurrentAf().low & Flag::Carry) != 0 ? 1 : 0);
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	UpdateZeroSignParityFlags(static_cast<uint8_t>(result));
	ClearFlag(Flag::AddSubtract);
}

void Z80Cpu::AndByteConst(uint8_t& byreRegister)
{
	auto value = FetchByte();
	AndByteRegister(byreRegister, value);
}

void Z80Cpu::AndByteRegister(uint8_t& byteRegister, uint8_t value)
{
	byteRegister &= value;
	UpdateZeroSignParityFlags(byteRegister);
	ClearFlag(Flag::Carry | Flag::AddSubtract);
}

void Z80Cpu::XorByteConst(uint8_t& byteRegister)
{
	auto value = FetchByte();
	XorByteRegister(byteRegister, value);
}

void Z80Cpu::XorByteRegister(uint8_t& byteRegister, uint8_t value)
{
	byteRegister ^= value;
	UpdateZeroSignParityFlags(byteRegister);
	ClearFlag(Flag::Carry | Flag::AddSubtract);
}

void Z80Cpu::OrByteConst(uint8_t& byteRegister)
{
	auto value = FetchByte();
	OrByteRegister(byteRegister, value);
}

void Z80Cpu::OrByteRegister(uint8_t& byteRegister, uint8_t value)
{
	byteRegister |= value;
	UpdateZeroSignParityFlags(byteRegister);
	ClearFlag(Flag::Carry | Flag::AddSubtract);
}

void Z80Cpu::CompareByteConst(uint8_t byteRegister)
{
	auto value = FetchByte();
	CompareByteRegister(byteRegister, value);
}

void Z80Cpu::CompareByteRegister(uint8_t byteRegister, uint8_t value)
{
	auto result = static_cast<int16_t>(byteRegister) - value;
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	UpdateZeroSignParityFlags(static_cast<uint8_t>(result));
	SetFlag(Flag::AddSubtract);
}

void Z80Cpu::Complement(uint8_t& byteRegister)
{
	byteRegister = ~byteRegister;
	SetFlag(Flag::AddSubtract | Flag::HalfCarry);
}

void Z80Cpu::Negate(uint8_t& byteRegiter)
{
	if ((byteRegiter & 0x0F) != 0) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	byteRegiter = -byteRegiter;
	if (byteRegiter != 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	UpdateZeroSignParityFlags(byteRegiter);
	SetFlag(Flag::AddSubtract);
}

void Z80Cpu::RotateLeftCircular(uint8_t& byteRegister)
{
	auto shifted = static_cast<uint16_t>(byteRegister) << 1;
	if (shifted & 0x100) {
		shifted |= 1;
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
	byteRegister = static_cast<uint8_t>(shifted);
}

void Z80Cpu::RotateLeftCircularMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	RotateLeftCircular(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::RotateLeft(uint8_t& byteRegister)
{
	auto shifted = static_cast<uint16_t>(byteRegister) << 1;
	if ((CurrentAf().low & Flag::Carry) != 0) {
		shifted |= 1;
	}
	if (shifted & 0x100) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	byteRegister = static_cast<uint8_t>(shifted);
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::RotateLeftMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	RotateLeft(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::RotateRightCircular(uint8_t& byteRegister)
{
	auto lsb = byteRegister & 1;
	byteRegister = (byteRegister >> 1) | (lsb << 7);
	if (lsb) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::RotateRightCircularMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	RotateRightCircular(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::RotateRight(uint8_t& byteRegister)
{
	auto lsb = byteRegister & 1;
	byteRegister = (byteRegister >> 1) | ((CurrentAf().low & Flag::Carry) << 7);
	if (lsb) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::RotateRightMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	RotateRight(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::ShiftLeftArithmetic(uint8_t& byteRegister)
{
	auto msb = byteRegister & 0x80;
	byteRegister <<= 1;
	if (msb) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::ShiftLeftArithmeticMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	ShiftLeftArithmetic(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::ShiftRightArithmetic(uint8_t& byteRegister)
{
	auto lsb = byteRegister & 1;
	byteRegister = (byteRegister >> 1) | (byteRegister & 0x80);
	if (lsb) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::ShiftRightArithmeticMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	ShiftRightArithmetic(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::ShiftLeftLogical(uint8_t& byteRegister)
{
	auto msb = byteRegister & 0x80;
	byteRegister <<= 1;
	if (msb) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::ShiftLeftLogicalMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	ShiftLeftLogical(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::ShiftRightLogical(uint8_t& byteRegister)
{
	auto lsb = byteRegister & 1;
	byteRegister >>= 1;
	if (lsb) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
}

void Z80Cpu::ShiftRightLogicalMemory(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	ShiftRightLogical(memoryValue);
	pMemorySpace->Write(address, memoryValue);
}

void Z80Cpu::RotateDigitRight(uint16_t address)
{
	auto memoryValue = pMemorySpace->Read(address);
	auto newA = ((memoryValue << 4) & 0xff) | (CurrentAf().high >> 4);
	auto newMemoryValue = (memoryValue >> 4) | ((CurrentAf().high << 4) & 0xff);
	CurrentAf().high = static_cast<uint8_t>(newA);
	pMemorySpace->Write(address, static_cast<uint8_t>(newMemoryValue));
	ClearFlag(Flag::Carry | Flag::HalfCarry | Flag::AddSubtract);
	UpdateZeroSignParityFlags(CurrentAf().high);
}

void Z80Cpu::DecimalAdjust(uint8_t& byteRegister)
{
	if ((CurrentAf().low & Flag::AddSubtract) == 0) {
		if ((CurrentAf().low & Flag::Carry) != 0 || byteRegister > 0x99) {
			byteRegister += 0x60;
			SetFlag(Flag::Carry);
		}
		else {
			ClearFlag(Flag::Carry);
		}
		if ((CurrentAf().low & Flag::HalfCarry) != 0 || (byteRegister & 0xf) > 0x9) {
			byteRegister += 0x6;
		}
	}
	else {
		if ((CurrentAf().low & Flag::Carry) != 0) {
			byteRegister -= 0x60;
		}
		if ((CurrentAf().low & Flag::HalfCarry) != 0) {
			byteRegister -= 0x6;
		}
	}
}

void Z80Cpu::LoadWord(uint16_t& wordRegister, uint16_t address)
{
	auto low = pMemorySpace->Read(address);
	auto high = pMemorySpace->Read(address + 1);
	wordRegister = MakeWord(high, low);
}

void Z80Cpu::LoadWordFromMemory(uint16_t& wordRegister)
{
	auto address = FetchWord();
	LoadWord(wordRegister, address);
}

void Z80Cpu::StoreWordToMemory(uint16_t wordRegister)
{
	auto address = FetchWord();
	StoreWord(address, wordRegister);
}

void Z80Cpu::AddWord(uint16_t& destination, uint16_t value)
{
	uint32_t result = static_cast<uint32_t>(destination) + value;
	if (result & 0x10000) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	ClearFlag(Flag::HalfCarry | Flag::AddSubtract);
	destination = static_cast<uint16_t>(result);
}

void Z80Cpu::AddWordCarry(uint16_t& destination, uint16_t value)
{
	auto result = static_cast<uint32_t>(destination) + value + ((CurrentAf().low & Flag::Carry) != 0 ? 1 : 0);
	if (result & 0x10000) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (result == 0) {
		SetFlag(Flag::Zero);
	}
	else {
		ClearFlag(Flag::Zero);
	}
	if (result & 0x1000) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	if ((result & 0x8000) != 0) {
		SetFlag(Flag::Sign);
	}
	else {
		ClearFlag(Flag::Sign);
	}
	if (result > 32767) {
		SetFlag(Flag::ParityOverflow);
	}
	else {
		ClearFlag(Flag::ParityOverflow);
	}
	ClearFlag(Flag::AddSubtract);
	destination = static_cast<uint16_t>(result);
}

void Z80Cpu::SubtractWordCarry(uint16_t& destination, const uint16_t value)
{
	int32_t result = static_cast<int32_t>(destination) - value - ((CurrentAf().low & Flag::Carry) != 0 ? 1 : 0);
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (result == 0) {
		SetFlag(Flag::Zero);
	}
	else {
		ClearFlag(Flag::Zero);
	}
	if (result & 0x1000) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	if ((result & 0x8000) != 0) {
		SetFlag(Flag::Sign);
	}
	else {
		ClearFlag(Flag::Sign);
	}
	if (result < -32768 || result > 32767) {
		SetFlag(Flag::ParityOverflow);
	}
	else {
		ClearFlag(Flag::ParityOverflow);
	}
	ClearFlag(Flag::AddSubtract);
	destination = static_cast<uint16_t>(result);
}

void Z80Cpu::ExchangeStack(uint16_t& wordRegister)
{
	auto temp = wordRegister;
	wordRegister = PopWord();
	PushWord(temp);
}

void Z80Cpu::ExchangeRegisters(uint16_t& wordRegister1, uint16_t& wordRegister2)
{
	auto temp = wordRegister1;
	wordRegister1 = wordRegister2;
	wordRegister2 = temp;
}

void Z80Cpu::JumpAbsolute(uint16_t address) {
	pc = address;
	FetchInstruction();
}

void Z80Cpu::JumpAbsolute()
{
	auto address = FetchWord();
	JumpAbsolute(address);
}

void Z80Cpu::JumpAbsolute(std::function<bool(uint8_t)> condition)
{
	if (condition(CurrentAf().low)) {
		JumpAbsolute();
	}
	else {
		pc += 2;
	}
}

void Z80Cpu::JumpRelative()
{
	auto offset = static_cast<int8_t>(FetchByte());
	pc += offset;
	FetchInstruction();
}

void Z80Cpu::JumpRelative(std::function<bool(uint8_t)> condition)
{
	auto offset = static_cast<int8_t>(FetchByte());
	if (condition(CurrentAf().low)) {
		clockCountToExecute += 5;
		pNextInstruction = &Instructions[0x18]; // JR e
	}
}

void Z80Cpu::DecrementJump(uint8_t& byteRegister)
{
	auto offset = static_cast<int8_t>(FetchByte());
	if (--byteRegister == 0) {
		clockCountToExecute += 5;
		pNextInstruction = &Instructions[0x18]; // JR e
	}
}

void Z80Cpu::Return()
{
	pc = PopWord();
	FetchInstruction();
}

void Z80Cpu::Return(std::function<bool(uint8_t)> condition)
{
	if (condition(CurrentAf().low)) {
		clockCountToExecute += 6;
		pNextInstruction = &Instructions[0xc9]; // RET
	}
}

void Z80Cpu::Call(uint16_t address) {
	PushWord(pc);
	pc = address;
	FetchInstruction();
}

void Z80Cpu::Call()
{
	uint16_t address = FetchWord();
	Call(address);
}

void Z80Cpu::Call(const std::function<bool(uint8_t)>& condition)
{
	if (condition(CurrentAf().low)) {
		clockCountToExecute += 7;
		pNextInstruction = &Instructions[0xcd]; // CALL nn
	}
}

void Z80Cpu::WriteIo(uint16_t address, const uint8_t byteRegister)
{
	pIoSpace->Write(address, byteRegister);
}

void Z80Cpu::WriteIo(const uint8_t byteRegister)
{
	pIoSpace->Write(FetchByte(), byteRegister);
}

void Z80Cpu::ReadIo(uint8_t& byteRegister, const uint16_t address)
{
	uint8_t value = pIoSpace->Read(address);
	byteRegister = value;
	UpdateZeroSignParityFlags(value);
}

void Z80Cpu::ReadIo(uint8_t& byteRegister)
{
	uint8_t low = FetchByte();
	auto address = MakeWord(CurrentAf().high, low);
	byteRegister = pIoSpace->Read(address);
}

void Z80Cpu::TestBit(int bit, uint8_t byteRegister)
{
	auto bitValue = (byteRegister >> bit) & 1;
	if (bitValue) {
		ClearFlag(Flag::Zero | Flag::ParityOverflow);
	}
	else {
		SetFlag(Flag::Zero | Flag::ParityOverflow);
	}
	if (bit == 7 && (byteRegister & 0x80)) {
		SetFlag(Flag::Sign);
	}
	else {
		ClearFlag(Flag::Sign);
	}
	SetFlag(Flag::HalfCarry);
	ClearFlag(Flag::AddSubtract);
}

void Z80Cpu::TestBitMemory(int bit, uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	TestBit(bit, value);
}

void Z80Cpu::ResetBit(int bit, uint8_t& byteRegister)
{
	byteRegister &= ~(1 << bit);
}

void Z80Cpu::ResetBitMemory(int bit, uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	ResetBit(bit, value);
	pMemorySpace->Write(address, value);
}

void Z80Cpu::SetBit(int bit, uint8_t& byteRegister)
{
	byteRegister |= (1 << bit);
}

void Z80Cpu::SetBitMemory(int bit, uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	SetBit(bit, value);
	pMemorySpace->Write(address, value);
}

void Z80Cpu::Reset()
{
	exchangeableRegisterIndex = 0;
	afIndex = 0;
	for (auto& exchangeableRegisters : exchangeableRegisterses) {
		exchangeableRegisters.bc.word = 0;
		exchangeableRegisters.de.word = 0;
		exchangeableRegisters.hl.word = 0;
	}
	for (auto& af : afs) {
		af.word = 0;
	}
	for (unsigned char& iff : iffs) {
		iff = 0;
	}
	ix.word = 0;
	iy.word = 0;
	sp = 0;
	pc = 0;
	halted = false;
	FetchInstruction();
}

void Z80Cpu::OnClock(uint32_t time)
{
	if (--clockCountToExecute == 0) {
		pNextInstruction->execute(*this);
		FetchInstruction();
	}
}
