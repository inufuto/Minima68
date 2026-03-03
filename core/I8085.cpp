#include "I8085.h"

#include <cassert>

#include "../platforms/windows/WinApi/ApiException.h"

static const std::function<void(I8085&)> Nop = [](I8085& cpu) {};
const I8085::Instruction I8085::Instructions[] = {
	{ 4, "NOP",Nop},
	{ 10,"LXI\tB,w", [](I8085& cpu)
	{
		/* 01 LXI B, nn */
		cpu.LoadWordConst(cpu.bc.word);
	} },
	{ 7,"STAX\tB", [](I8085& cpu)
	{
		/* 02 STAX B */
		cpu.StoreByte(cpu.bc.word, cpu.af.high);
	} },
	{ 6,"INX\tB", [](I8085& cpu)
	{
		/* 03 INX B */
		cpu.IncreaseWord(cpu.bc.word, 1);
	} },
	{ 4,"INR\tB", [](I8085& cpu)
	{
		/* 04 INR B */
		cpu.IncreaseByte(cpu.bc.high, 1);
	} },
	{ 4,"DCR\tB", [](I8085& cpu)
	{
		/* 05 DCR B */
		cpu.IncreaseByte(cpu.bc.high, -1);
	} },
	{ 7,"MVI\tB,b", [](I8085& cpu)
	{
		/* 06 MVI B, n */
		cpu.MoveImmediate(cpu.bc.high);
	} },
	{4,"RLC",[](I8085& cpu)
	{
		/* 07 RLC */
		cpu.RotateLeftCircular(cpu.af.high);
	}},
	{10,"DSUB",[](I8085& cpu)
	{
		/* 08 *DSUB */
		cpu.SubtractWord(cpu.hl.word, cpu.bc.word);
	}},
	{ 10,"DAD\tB", [](I8085& cpu)
	{
		/* 09 DAD B */
		cpu.AddWord(cpu.hl.word, cpu.bc.word);
	} },
	{ 7,"LDAX\tB", [](I8085& cpu)
	{
		/* 0a LDAX B */
		cpu.LoadByte(cpu.af.high, cpu.bc.word);
	} },
	{6,"DCX\tB",[](I8085& cpu)
	{
		/* 0b DCX B */
		cpu.IncreaseWord(cpu.bc.word, -1);
	}},
	{ 4,"INR\tC",[](I8085& cpu)
	{
		/* 0c INR C */
		cpu.IncreaseByte(cpu.bc.low, 1);
	}},
	{ 4,"DCR\tC",[](I8085& cpu)
	{
		/* 0d DCR C */
		cpu.IncreaseByte(cpu.bc.low, -1);
	}},
	{ 7,"MVI\tC,b", [](I8085& cpu)
	{
		/* 0e MVI C, n */
		cpu.MoveImmediate(cpu.bc.low);
	} },
	{ 4,"RRC", [](I8085& cpu)
	{
		/* 0f RRC */
		cpu.RotateRightCircular(cpu.af.high);
	} },
	{7,"ARHL",[](I8085& cpu)
	{
		/* 10 *ARHL */
		cpu.ShiftWordRightArithmetic(cpu.hl.word);
	}},
	{10,"LXI\tD,w",[](I8085& cpu)
	{
		/* 11 LXI D, nn */
		cpu.LoadWordConst(cpu.de.word);
	}},
	{7,"STAX\tD",[](I8085& cpu)
	{
		/* 12 STAX D */
		cpu.StoreByte(cpu.de.word, cpu.af.high);
	}},
	{6,"INX\tD",[](I8085& cpu)
	{
		/* 13 INX D */
		cpu.IncreaseWord(cpu.de.word, 1);
	}},
	{4,"INR\tD",[](I8085& cpu)
	{
		/* 14 INR D */
		cpu.IncreaseByte(cpu.de.high, 1);
	}},
	{4,"DCR\tD",[](I8085& cpu)
	{
		/* 15 DCR D */
		cpu.IncreaseByte(cpu.de.high, -1);
	}},
	{7,"MVI\tD,b", [](I8085& cpu)
	{
		/* 16 MVI D, n */
		cpu.MoveImmediate(cpu.de.high);
	} },
	{4,"RAL",[](I8085& cpu)
	{
		/* 17 RAL */
		cpu.RotateLeft(cpu.af.high);
	} },
	{10,"RDEL",[](I8085& cpu)
	{
		/* 18 *RDEL */
		cpu.RotateWordLeft(cpu.de.word);
	}},
	{10,"DAD\tD",[](I8085& cpu)
	{
		/* 19 DAD D */
		cpu.AddWord(cpu.hl.word, cpu.de.word);
	}},
	{7,"LDAX\tD",[](I8085& cpu)
	{
		/* 1a LDAX D */
		cpu.LoadByte(cpu.af.high, cpu.de.word);
	}},
	{6,"DCX\tD",[](I8085& cpu)
	{
		/* 1b DCX D */
		cpu.IncreaseWord(cpu.de.word, -1);
	}},
	{4,"INR\tE",[](I8085& cpu)
	{
		/* 1c INR E */
		cpu.IncreaseByte(cpu.de.low, 1);
	}},
	{4,"DCR\tE",[](I8085& cpu)
	{
		/* 1d DCR E */
		cpu.IncreaseByte(cpu.de.low, -1);
	}},
	{7,"MVI\tE,b", [](I8085& cpu)
	{
		/* 1e MVI E, n */
		cpu.MoveImmediate(cpu.de.low);
	} },
	{4,"RAR",[](I8085& cpu)
	{
		/* 1f RAR */
		cpu.RotateRight(cpu.af.high);
	} },
	{4,"RIM",[](I8085& cpu)
	{
		/* 20 RIM */
		cpu.ReadInterruptBits(cpu.af.high);
	}},
	{10,"LXI\tH,w",[](I8085& cpu)
	{
		/* 21 LXI H, nn */
		cpu.LoadWordConst(cpu.hl.word);
	}},
	{7,"SHLD\tw",[](I8085& cpu)
	{
		/* 22 SHLD nn */
		cpu.StoreWord(cpu.FetchWord(), cpu.hl.word);
	}},
	{6,"INX\tH",[](I8085& cpu)
	{
		/* 23 INX H */
		cpu.IncreaseWord(cpu.hl.word, 1);
	}},
	{4,"INR\tH",[](I8085& cpu)
	{
		/* 24 INR H */
		cpu.IncreaseByte(cpu.hl.high, 1);
	}},
	{4,"DCR\tH",[](I8085& cpu)
	{
		/* 25 DCR H */
		cpu.IncreaseByte(cpu.hl.high, -1);
	}},
	{7,"MVI\tH,b", [](I8085& cpu)
	{
		/* 26 MVI H, n */
		cpu.MoveImmediate(cpu.hl.high);
	} },
	{4,"DAA",[](I8085& cpu)
	{
		/* 27 DAA */
		cpu.DecimalAdjust(cpu.af.high);
	} },
	{10,"LDHI\tb",[](I8085& cpu)
	{
		/* 28 *LDHI */
		cpu.AddWordByteConst(cpu.de.word, cpu.hl.word);
	}},
	{10,"DAD\tH",[](I8085& cpu)
	{
		/* 29 DAD H */
		cpu.AddWord(cpu.hl.word, cpu.hl.word);
	}},
	{7,"LHLD\tw",[](I8085& cpu)
	{
		/* 2a LHLD nn */
		cpu.LoadWordMemory(cpu.hl.word);
	}},
	{6,"DCX\tH",[](I8085& cpu)
	{
		/* 2b DCX H */
		cpu.IncreaseWord(cpu.hl.word, -1);
	}},
	{4,"INR\tL",[](I8085& cpu)
	{
		/* 2c INR L */
		cpu.IncreaseByte(cpu.hl.low, 1);
	}},
	{4,"DCR\tL",[](I8085& cpu)
	{
		/* 2d DCR L */
		cpu.IncreaseByte(cpu.hl.low, -1);
	}},
	{7,"MVI\tL,b", [](I8085& cpu)
	{
		/* 2e MVI L, n */
		cpu.MoveImmediate(cpu.hl.low);
	} },
	{4,"CMA",[](I8085& cpu)
	{
		/* 2f CMA */
		cpu.af.high = ~cpu.af.high;
	} },
	{ 4,"SIM",[](I8085& cpu)
	{
		/* 30 SIM */
		cpu.WriteInterruptBits(cpu.af.high);
	}},
	{10,"LXI\tSP,w",[](I8085& cpu)
	{
		/* 31 LXI SP, nn */
		cpu.LoadWordConst(cpu.sp);
	}},
	{7,"STA\tw",[](I8085& cpu)
	{
		/* 32 STA nn */
		cpu.StoreByte(cpu.af.high);
	}},
	{6,"INX\tSP",[](I8085& cpu)
	{
		/* 33 INX SP */
		cpu.IncreaseWord(cpu.sp, 1);
	}},
	{4,"INR\tM",[](I8085& cpu)
	{
		/* 34 INR M */
		cpu.IncreaseMemory(cpu.hl.word, 1);
	}},
	{4,"DCR\tM",[](I8085& cpu)
	{
		/* 35 DCR M */
		cpu.IncreaseMemory(cpu.hl.word,-1);
	}},
	{7,"MVI\tM,b", [](I8085& cpu)
	{
		/* 36 MVI M, n */
		cpu.MoveImmediateMemory(cpu.hl.word);
	} },
	{4,"STC",[](I8085& cpu)
	{
		/* 37 STC */
		cpu.SetFlag(Flag::Carry);
	} },
	{10,"LDSI",[](I8085& cpu)
	{
		/* 38 *LDSI */
		cpu.AddWordByteConst(cpu.de.word, cpu.sp);
	}},
	{10,"DAD\tSP",[](I8085& cpu)
	{
		/* 39 DAD SP */
		cpu.AddWord(cpu.hl.word, cpu.sp);
	} },
	{7,"LDA\tw",[](I8085& cpu)
	{
		/* 3a LDA nn */
		cpu.LoadByte(cpu.af.high);
	} },
	{6,"DCX\tSP",[](I8085& cpu)
	{
		/* 3b DCX SP */
		cpu.IncreaseWord(cpu.sp, -1);
	}},
	{4,"INR\tA",[](I8085& cpu)
	{
		/* 3c INR A */
		cpu.IncreaseByte(cpu.af.high, 1);
	}},
	{4,"DCR\tA",[](I8085& cpu)
	{
		/* 3d DCR A */
		cpu.IncreaseByte(cpu.af.high, -1);
	}},
	{ 7,"MVI\tA,b", [](I8085& cpu)
	{
		/* 3e MVI A, n */
		cpu.MoveImmediate(cpu.af.high);
	} },
	{ 4,"CMC",[](I8085& cpu)
	{
		/* 3f CMC */
		cpu.ComplementFlag(Flag::Carry);
	} },
	{4,"MOV\tB,B",[](I8085& cpu)
	{
		/* 40 MOV B, B */
		Move(cpu.bc.high, cpu.bc.high);
	}},
	{4,"MOV\tB,C",[](I8085& cpu)
	{
		/* 41 MOV B, C */
		Move(cpu.bc.high, cpu.bc.low);
	}},
	{4,"MOV\tB,D",[](I8085& cpu)
	{
		/* 42 MOV B, D */
		Move(cpu.bc.high, cpu.de.high);
	}},
	{4,"MOV\tB,E",[](I8085& cpu)
	{
		/* 43 MOV B, E */
		Move(cpu.bc.high, cpu.de.low);
	}},
	{4,"MOV\tB,H",[](I8085& cpu)
	{
		/* 44 MOV B, H */
		Move(cpu.bc.high, cpu.hl.high);
	}},
	{4,"MOV\tB,L",[](I8085& cpu)
	{
		/* 45 MOV B, L */
		Move(cpu.bc.high, cpu.hl.low);
	} },
	{7,"MOV\tB,M",[](I8085& cpu)
	{
		/* 46 MOV B, M */
		cpu.LoadByte(cpu.bc.high, cpu.hl.word);
	}},
	{4,"MOV\tB,A",[](I8085& cpu)
	{
		/* 47 MOV B, A */
		Move(cpu.bc.high, cpu.af.high);
	} },
	{4,"MOV\tC,B",[](I8085& cpu)
	{
		/* 48 MOV C, B */
		Move(cpu.bc.low, cpu.bc.high);
	}},
	{4,"MOV\tC,C",[](I8085& cpu)
	{
		/* 49 MOV C, C */
		Move(cpu.bc.low, cpu.bc.low);
	}},
	{4,"MOV\tC,D",[](I8085& cpu)
	{
		/* 4a MOV C, D */
		Move(cpu.bc.low, cpu.de.high);
	}},
	{4,"MOV\tC,E",[](I8085& cpu)
	{
		/* 4b MOV C, E */
		Move(cpu.bc.low, cpu.de.low);
	}},
	{4,"MOV\tC,H",[](I8085& cpu)
	{
		/* 4c MOV C, H */
		Move(cpu.bc.low, cpu.hl.high);
	}},
	{4,"MOV\tC,L",[](I8085& cpu)
	{
		/* 4d MOV C, L */
		Move(cpu.bc.low, cpu.hl.low);
	} },
	{7,"MOV\tC,M",[](I8085& cpu)
	{
		/* 4e MOV C, M */
		cpu.LoadByte(cpu.bc.low, cpu.hl.word);
	}},
	{ 4,"MOV\tC,A",[](I8085& cpu)
	{
		/* 4f MOV C, A */
		Move(cpu.bc.low, cpu.af.high);
	} },
	{4,"MOV\tD,B",[](I8085& cpu)
	{
		/* 50 MOV D, B */
		Move(cpu.de.high, cpu.bc.high);
	} },
	{4,"MOV\tD,C",[](I8085& cpu)
	{
		/* 51 MOV D, C */
		Move(cpu.de.high, cpu.bc.low);
	}},
	{4,"MOV\tD,D",[](I8085& cpu)
	{
		/* 52 MOV D, D */
		Move(cpu.de.high, cpu.de.high);
	}},
	{4,"MOV\tD,E",[](I8085& cpu)
	{
		/* 53 MOV D, E */
		Move(cpu.de.high, cpu.de.low);
	}},
	{4,"MOV\tD,H",[](I8085& cpu)
	{
		/* 54 MOV D, H */
		Move(cpu.de.high, cpu.hl.high);
	}},
	{4,"MOV\tD,L",[](I8085& cpu)
	{
		/* 55 MOV D, L */
		Move(cpu.de.high, cpu.hl.low);
	} },
	{7,"MOV\tD,M",[](I8085& cpu)
	{
		/* 56 MOV D, M */
		cpu.LoadByte(cpu.de.high, cpu.hl.word);
	}},
	{4,"MOV\tD,A",[](I8085& cpu)
	{
		/* 57 MOV D, A */
		Move(cpu.de.high, cpu.af.high);
	} },
	{4,"MOV\tE,B",[](I8085& cpu)
	{
		/* 58 MOV E, B */
		Move(cpu.de.low, cpu.bc.high);
	}},
	{4,"MOV\tE,C",[](I8085& cpu)
	{
		/* 59 MOV E, C */
		Move(cpu.de.low, cpu.bc.low);
	}},
	{4,"MOV\tE,D",[](I8085& cpu)
	{
		/* 5a MOV E, D */
		Move(cpu.de.low, cpu.de.high);
	}},
	{4,"MOV\tE,E",[](I8085& cpu)
	{
		/* 5b MOV E, E */
		Move(cpu.de.low, cpu.de.low);
	}},
	{4,"MOV\tE,H",[](I8085& cpu)
	{
		/* 5c MOV E, H */
		Move(cpu.de.low, cpu.hl.high);
	}},
	{4,"MOV\tE,L",[](I8085& cpu)
	{
		/* 5d MOV E, L */
		Move(cpu.de.low, cpu.hl.low);
	} },
	{7,"MOV\tE,M",[](I8085& cpu)
	{
		/* 5e MOV E, M */
		cpu.LoadByte(cpu.de.low, cpu.hl.word);
	}},
	{4,"MOV\tE,A",[](I8085& cpu)
	{
		/* 5f MOV E, A */
		Move(cpu.de.low, cpu.af.high);
	} },
	{4,"MOV\tH,B",[](I8085& cpu)
	{
		/* 60 MOV H, B */
		Move(cpu.hl.high, cpu.bc.high);
	}},
	{4,"MOV\tH,C",[](I8085& cpu)
	{
		/* 61 MOV H, C */
		Move(cpu.hl.high, cpu.bc.low);
	}},
	{4,"MOV\tH,D",[](I8085& cpu)
	{
		/* 62 MOV H, D */
		Move(cpu.hl.high, cpu.de.high);
	}},
	{4,"MOV\tH,E",[](I8085& cpu)
	{
		/* 63 MOV H, E */
		Move(cpu.hl.high, cpu.de.low);
	}},
	{4,"MOV\tH,H",[](I8085& cpu)
	{
		/* 64 MOV H, H */
		Move(cpu.hl.high, cpu.hl.high);
	}},
	{4,"MOV\tH,L",[](I8085& cpu)
	{
		/* 65 MOV H, L */
		Move(cpu.hl.high, cpu.hl.low);
	}},
	{7,"MOV\tH,M",[](I8085& cpu)
	{
		/* 66 MOV H, M */
		cpu.LoadByte(cpu.hl.high, cpu.hl.word);
	}},
	{4,"MOV\tH,A",[](I8085& cpu)
	{
		/* 67 MOV H, A */
		Move(cpu.hl.high, cpu.af.high);
	}},
	{4,"MOV\tL,B",[](I8085& cpu)
	{
		/* 68 MOV L, B */
		Move(cpu.hl.low, cpu.bc.high);
	}},
	{4,"MOV\tL,C",[](I8085& cpu)
	{
		/* 69 MOV L, C */
		Move(cpu.hl.low, cpu.bc.low);
	}},
	{4,"MOV\tL,D",[](I8085& cpu)
	{
		/* 6a MOV L, D */
		Move(cpu.hl.low, cpu.de.high);
	}},
	{4,"MOV\tL,E",[](I8085& cpu)
	{
		/* 6b MOV L, E */
		Move(cpu.hl.low, cpu.de.low);
	}},
	{4,"MOV\tL,H",[](I8085& cpu)
	{
		/* 6c MOV L, H */
		Move(cpu.hl.low, cpu.hl.high);
	}},
	{4,"MOV\tL,L",[](I8085& cpu)
	{
		/* 6d MOV L, L */
		Move(cpu.hl.low, cpu.hl.low);
	}},
	{7,"MOV\tL,M",[](I8085& cpu)
	{
		/* 6e MOV L, M */
		cpu.LoadByte(cpu.hl.low, cpu.hl.word);
	}},
	{4,"MOV\tL,A",[](I8085& cpu)
	{
		/* 6f MOV L, A */
		Move(cpu.hl.low, cpu.af.high);
	}},
	{7,"MOV M,B",[](I8085& cpu)
	{
		/* 70 MOV M, B */
		cpu.StoreByte(cpu.hl.word, cpu.bc.high);
	}},
	{7,"MOV M,C",[](I8085& cpu)
	{
		/* 71 MOV M, C */
		cpu.StoreByte(cpu.hl.word, cpu.bc.low);
	}},
	{7,"MOV M,D",[](I8085& cpu)
	{
		/* 72 MOV M, D */
		cpu.StoreByte(cpu.hl.word, cpu.de.high);
	}},
	{7,"MOV M,E",[](I8085& cpu)
	{
		/* 73 MOV M, E */
		cpu.StoreByte(cpu.hl.word, cpu.de.low);
	}},
	{7,"MOV M,H",[](I8085& cpu)
	{
		/* 74 MOV M, H */
		cpu.StoreByte(cpu.hl.word, cpu.hl.high);
	}},
	{7,"MOV M,L",[](I8085& cpu)
	{
		/* 75 MOV M, L */
		cpu.StoreByte(cpu.hl.word, cpu.hl.low);
	}},
	{5,"HLT",[](I8085& cpu)
	{
		/* 76 HLT */
		cpu.Halt();
	}},
	{ 7,"MOV M,A",[](I8085& cpu)
	{
		/* 77 MOV M, A */
		cpu.StoreByte(cpu.hl.word, cpu.af.high);
	} },
	{4,"MOV\tA,B",[](I8085& cpu)
	{
		/* 78 MOV A, B */
		Move(cpu.af.high, cpu.bc.high);
	}},
	{4,"MOV\tA,C",[](I8085& cpu)
	{
		/* 79 MOV A, C */
		Move(cpu.af.high, cpu.bc.low);
	}},
	{4,"MOV\tA,D",[](I8085& cpu)
	{
		/* 7a MOV A, D */
		Move(cpu.af.high, cpu.de.high);
	}},
	{4,"MOV\tA,E",[](I8085& cpu)
	{
		/* 7b MOV A, E */
		Move(cpu.af.high, cpu.de.low);
	}},
	{4,"MOV\tA,H",[](I8085& cpu)
	{
		/* 7c MOV A, H */
		Move(cpu.af.high, cpu.hl.high);
	}},
	{4,"MOV\tA,L",[](I8085& cpu)
	{
		/* 7d MOV A, L */
		Move(cpu.af.high, cpu.hl.low);
	} },
	{7,"MOV\tA,M",[](I8085& cpu)
	{
		/* 7e MOV A, M */
		cpu.LoadByte(cpu.af.high, cpu.hl.word);
	}},
	{ 4,"MOV\tA,A",[](I8085& cpu)
	{
		/* 7f MOV A, A */
		Move(cpu.af.high, cpu.af.high);
	} },
	{4,"ADD\tB",[](I8085& cpu)
	{
		/* 80 ADD B */
		cpu.AddByte(cpu.af.high, cpu.bc.high);
	}},
	{4,"ADD\tC",[](I8085& cpu)
	{
		/* 81 ADD C */
		cpu.AddByte(cpu.af.high, cpu.bc.low);
	}},
	{4,"ADD\tD",[](I8085& cpu)
	{
		/* 82 ADD D */
		cpu.AddByte(cpu.af.high, cpu.de.high);
	}},
	{4,"ADD\tE",[](I8085& cpu)
	{
		/* 83 ADD E */
		cpu.AddByte(cpu.af.high, cpu.de.low);
	}},
	{4,"ADD\tH",[](I8085& cpu)
	{
		/* 84 ADD H */
		cpu.AddByte(cpu.af.high, cpu.hl.high);
	}},
	{4,"ADD\tL",[](I8085& cpu)
	{
		/* 85 ADD L */
		cpu.AddByte(cpu.af.high, cpu.hl.low);
	}},
	{7,"ADD\tM",[](I8085& cpu)
	{
		/* 86 ADD M */
		cpu.AddByteMemory(cpu.af.high, cpu.hl.word);
	}},
	{4,"ADD\tA",[](I8085& cpu)
	{
		/* 87 ADD A */
		cpu.AddByte(cpu.af.high, cpu.af.high);
	} },
	{4,"ADC\tB",[](I8085& cpu)
	{
		/* 88 ADC B */
		cpu.AddByteWithCarry(cpu.af.high, cpu.bc.high);
	}},
	{4,"ADC\tC",[](I8085& cpu)
	{
		/* 89 ADC C */
		cpu.AddByteWithCarry(cpu.af.high, cpu.bc.low);
	}},
	{4,"ADC\tD",[](I8085& cpu)
	{
		/* 8a ADC D */
		cpu.AddByteWithCarry(cpu.af.high, cpu.de.high);
	}},
	{4,"ADC\tE",[](I8085& cpu)
	{
		/* 8b ADC E */
		cpu.AddByteWithCarry(cpu.af.high, cpu.de.low);
	}},
	{4,"ADC\tH",[](I8085& cpu)
	{
		/* 8c ADC H */
		cpu.AddByteWithCarry(cpu.af.high, cpu.hl.high);
	}},
	{4,"ADC\tL",[](I8085& cpu)
	{
		/* 8d ADC L */
		cpu.AddByteWithCarry(cpu.af.high, cpu.hl.low);
	}},
	{7,"ADC\tM",[](I8085& cpu)
	{
		/* 8e ADC M */
		cpu.AddByteMemoryWithCarry(cpu.af.high, cpu.hl.word);
	}},
	{4,"ADC\tA",[](I8085& cpu)
	{
		/* 8f ADC A */
		cpu.AddByteWithCarry(cpu.af.high, cpu.af.high);
	} },
	{4,"SUB\tB",[](I8085& cpu)
	{
		/* 90 SUB B */
		cpu.SubtractByte(cpu.af.high, cpu.bc.high);
	}},
		{4,"SUB\tC",[](I8085& cpu)
	{
		/* 91 SUB C */
		cpu.SubtractByte(cpu.af.high, cpu.bc.low);
	}},
	{4,"SUB\tD",[](I8085& cpu)
	{
		/* 92 SUB D */
		cpu.SubtractByte(cpu.af.high, cpu.de.high);
	}},
	{4,"SUB\tE",[](I8085& cpu)
	{
		/* 93 SUB E */
		cpu.SubtractByte(cpu.af.high, cpu.de.low);
	}},
	{4,"SUB\tH",[](I8085& cpu)
	{
		/* 94 SUB H */
		cpu.SubtractByte(cpu.af.high, cpu.hl.high);
	}},
	{4,"SUB\tL",[](I8085& cpu)
	{
		/* 95 SUB L */
		cpu.SubtractByte(cpu.af.high, cpu.hl.low);
	}},
	{7,"SUB\tM",[](I8085& cpu)
	{
		/* 96 SUB M */
		cpu.SubtractByteMemory(cpu.af.high, cpu.hl.word);
	}},
	{4,"SUB\tA",[](I8085& cpu)
	{
		/* 97 SUB A */
		cpu.SubtractByte(cpu.af.high, cpu.af.high);
	} },
	{4,"SBB\tB",[](I8085& cpu)
	{
		/* 98 SBB B */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.bc.high);
	}},
	{4,"SBB\tC",[](I8085& cpu)
	{
		/* 99 SBB C */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.bc.low);
	}},
	{4,"SBB\tD",[](I8085& cpu)
	{
		/* 9a SBB D */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.de.high);
	}},
	{4,"SBB\tE",[](I8085& cpu)
	{
		/* 9b SBB E */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.de.low);
	}},
	{4,"SBB\tH",[](I8085& cpu)
	{
		/* 9c SBB H */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.hl.high);
	}},
	{4,"SBB\tL",[](I8085& cpu)
	{
		/* 9d SBB L */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.hl.low);
	}},
	{7,"SBB\tM",[](I8085& cpu)
	{
		/* 9e SBB M */
		cpu.SubtractByteMemoryWithCarry(cpu.af.high, cpu.hl.word);
	}},
	{4,"SBB\tA",[](I8085& cpu)
	{
		/* 9f SBB A */
		cpu.SubtractByteWithCarry(cpu.af.high, cpu.af.high);
	} },
	{4,"ANA\tB",[](I8085& cpu)
	{
		/* a0 ANA B */
		cpu.AndByte(cpu.af.high, cpu.bc.high);
	}},
	{4,"ANA\tC",[](I8085& cpu)
	{
		/* a1 ANA C */
		cpu.AndByte(cpu.af.high, cpu.bc.low);
	}},
	{4,"ANA\tD",[](I8085& cpu)
	{
		/* a2 ANA D */
		cpu.AndByte(cpu.af.high, cpu.de.high);
	}},
	{4,"ANA\tE",[](I8085& cpu)
	{
		/* a3 ANA E */
		cpu.AndByte(cpu.af.high, cpu.de.low);
	}},
	{4,"ANA\tH",[](I8085& cpu)
	{
		/* a4 ANA H */
		cpu.AndByte(cpu.af.high, cpu.hl.high);
	}},
	{4,"ANA\tL",[](I8085& cpu)
	{
		/* a5 ANA L */
		cpu.AndByte(cpu.af.high, cpu.hl.low);
	}},
	{7,"ANA\tM",[](I8085& cpu)
	{
		/* a6 ANA M */
		cpu.AndByteMemory(cpu.af.high, cpu.hl.word);
	}},
	{4,"ANA\tA",[](I8085& cpu)
	{
		/* a7 ANA A */
		cpu.AndByte(cpu.af.high, cpu.af.high);
	} },
	{4,"XRA\tB",[](I8085& cpu)
	{
		/* a8 XRA B */
		cpu.XorByte(cpu.af.high, cpu.bc.high);
	}},
	{4,"XRA\tC",[](I8085& cpu)
	{
		/* a9 XRA C */
		cpu.XorByte(cpu.af.high, cpu.bc.low);
	}},
	{4,"XRA\tD",[](I8085& cpu)
	{
		/* aa XRA D */
		cpu.XorByte(cpu.af.high, cpu.de.high);
	}},
	{4,"XRA\tE",[](I8085& cpu)
	{
		/* ab XRA E */
		cpu.XorByte(cpu.af.high, cpu.de.low);
	}},
	{4,"XRA\tH",[](I8085& cpu)
	{
		/* ac XRA H */
		cpu.XorByte(cpu.af.high, cpu.hl.high);
	}},
	{4,"XRA\tL",[](I8085& cpu)
	{
		/* ad XRA L */
		cpu.XorByte(cpu.af.high, cpu.hl.low);
	}},
	{7,"XRA\tM",[](I8085& cpu)
	{
		/* ae XRA M */
		cpu.XorByteMemory(cpu.af.high, cpu.hl.word);
	}},
	{4,"XRA\tA",[](I8085& cpu)
	{
		/* af XRA A */
		cpu.XorByte(cpu.af.high, cpu.af.high);
	} },
	{4,"ORA\tB",[](I8085& cpu)
	{
		/* b0 ORA B */
		cpu.OrByte(cpu.af.high, cpu.bc.high);
	}},
	{4,"ORA\tC",[](I8085& cpu)
	{
		/* b1 ORA C */
		cpu.OrByte(cpu.af.high, cpu.bc.low);
	}},
	{4,"ORA\tD",[](I8085& cpu)
	{
		/* b2 ORA D */
		cpu.OrByte(cpu.af.high, cpu.de.high);
	}},
	{4,"ORA\tE",[](I8085& cpu)
	{
		/* b3 ORA E */
		cpu.OrByte(cpu.af.high, cpu.de.low);
	}},
	{4,"ORA\tH",[](I8085& cpu)
	{
		/* b4 ORA H */
		cpu.OrByte(cpu.af.high, cpu.hl.high);
	}},
	{4,"ORA\tL",[](I8085& cpu)
	{
		/* b5 ORA L */
		cpu.OrByte(cpu.af.high, cpu.hl.low);
	}},
	{7,"ORA\tM",[](I8085& cpu)
	{
		/* b6 ORA M */
		cpu.OrByteMemory(cpu.af.high, cpu.hl.word);
	}},
	{4,"ORA\tA",[](I8085& cpu)
	{
		/* b7 ORA A */
		cpu.OrByte(cpu.af.high, cpu.af.high);
	} },
	{4,"CMP\tB",[](I8085& cpu)
	{
		/* b8 CMP B */
		cpu.CompareByte(cpu.af.high, cpu.bc.high);
	}},
	{4,"CMP\tC",[](I8085& cpu)
	{
		/* b9 CMP C */
		cpu.CompareByte(cpu.af.high, cpu.bc.low);
	}},
	{4,"CMP\tD",[](I8085& cpu)
	{
		/* ba CMP D */
		cpu.CompareByte(cpu.af.high, cpu.de.high);
	}},
	{4,"CMP\tE",[](I8085& cpu)
	{
		/* bb CMP E */
		cpu.CompareByte(cpu.af.high, cpu.de.low);
	}},
	{4,"CMP\tH",[](I8085& cpu)
	{
		/* bc CMP H */
		cpu.CompareByte(cpu.af.high, cpu.hl.high);
	}},
	{4,"CMP\tL",[](I8085& cpu)
	{
		/* bd CMP L */
		cpu.CompareByte(cpu.af.high, cpu.hl.low);
	}},
	{7,"CMP\tM",[](I8085& cpu)
	{
		/* be CMP M */
		cpu.CompareByteMemory(cpu.af.high, cpu.hl.word);
	}},
	{4,"CMP\tA",[](I8085& cpu)
	{
		/* bf CMP A */
		cpu.CompareByte(cpu.af.high, cpu.af.high);
	} },
	{6 /* or 12 */,"RNZ",[](I8085& cpu)
	{
		/* c0 RNZ */
		cpu.ReturnIf([](const uint8_t flags) {
			return !(flags & Flag::Zero);
		});
	}},
	{10,"POP\tB",[](I8085& cpu)
	{
		/* c1 POP B */
		cpu.bc.word = cpu.PopWord();
	}}
	,
	{7 /* or 10 */,"JNZ\tw",[](I8085& cpu)
	{
		/* c2 JNZ nn */
		cpu.JumpIf([](const uint8_t flags) {
			return !(flags & Flag::Zero);
		});
	}},
	{10,"JMP\tw",[](I8085& cpu)
	{
		/* c3 JMP nn */
		cpu.Jump();
	}},
	{9 /* or 18 */,"CNZ\tw",[](I8085& cpu)
	{
		/* c4 CNZ nn */
		cpu.CallIf([](const uint8_t flags) {
			return !(flags & Flag::Zero);
		});
	} },
	{12,"PUSH\tB",[](I8085& cpu)
	{
		/* c5 PUSH B */
		cpu.PushWord(cpu.bc.word);
	}},
	{7 /* or 11 */,"ADI\tb",[](I8085& cpu)
	{
		/* c6 ADI n */
		cpu.AddByteImmediate(cpu.af.high);
	}},
	{12,"RST\t0",[](I8085& cpu)
	{
		/* c7 RST 0 */
		cpu.Call(0x00);
	} },
	{6 /* or 12 */,"RZ",[](I8085& cpu)
	{
		/* c8 RZ */
		cpu.ReturnIf([](const uint8_t flags) {
			return (flags & Flag::Zero) != 0;
		});
	}},
	{10,"RET",[](I8085& cpu)
	{
		/* c9 RET */
		cpu.Return();
	}},
	{7 /* or 10 */,"JZ\tw",[](I8085& cpu)
	{
		/* ca JZ nn */
		cpu.JumpIf([](const uint8_t flags) {
			return (flags & Flag::Zero) != 0;
		});
	}},
	{6 /* or 12 */,"RSTV",[](I8085& cpu)
	{
		/* cb *RSTV */
		cpu.RestartIfOverflow();
	} },
	{ 9 /* or 18 */, "CZ\tw",[](I8085& cpu)
	{
		/* cc CZ nn */
		cpu.CallIf([](const uint8_t flags) {
			return (flags & Flag::Zero) != 0;
		});
	}},
		{12,"CALL\tw",[](I8085& cpu)
	{
		/* cd CALL nn */
		cpu.Call();
	}},
	{7 /* or 11 */,"ACI\tb",[](I8085& cpu)
	{
		/* ce ACI n */
		cpu.AddByteImmediateWithCarry(cpu.af.high);
	}},
	{12,"RST\t1",[](I8085& cpu)
	{
		/* cf RST 1 */
		cpu.Call(0x08);
	} },
	{ 6 /* or 12 */,"RNC",[](I8085& cpu)
	{
		/* d0 RNC */
		cpu.ReturnIf([](const uint8_t flags) {
			return !(flags & Flag::Carry);
		});
	} },
	{10,"POP\tD",[](I8085& cpu)
	{
		/* d1 POP D */
		cpu.de.word = cpu.PopWord();
	}},
	{7 /* or 10 */,"JNC\tw",[](I8085& cpu)
	{
		/* d2 JNC nn */
		cpu.JumpIf([](const uint8_t flags) {
			return !(flags & Flag::Carry);
		});
	}},
	{ 10,"OUT\tb",[](I8085& cpu)
	{
		/* d3 OUT n */
		cpu.Output(cpu.af.high);
	}},
	{9 /* or 18 */,"CNC\tw",[](I8085& cpu)
	{
		/* d4 CNC nn */
		cpu.CallIf([](const uint8_t flags) {
			return !(flags & Flag::Carry);
		});
	} },
	{12,"PUSH\tD",[](I8085& cpu)
	{
		/* d5 PUSH D */
		cpu.PushWord(cpu.de.word);
	}},
	{7,"SUI\tb",[](I8085& cpu)
	{
		/* d6 SUI n */
		cpu.SubtractByteImmediate(cpu.af.high);
	}},
	{12,"RST\t2",[](I8085& cpu)
	{
		/* d7 RST 2 */
		cpu.Call(0x10);
	} },
	{6 /* or 12 */,"RC",[](I8085& cpu)
	{
		/* d8 RC */
		cpu.ReturnIf([](const uint8_t flags) {
			return (flags & Flag::Carry) != 0;
		});
	} },
	{10,"SHLX", [](I8085& cpu)
	{
		/* d9 *SHLX */
		cpu.StoreWord(cpu.de.word, cpu.hl.word);
	}},
	{7 /* or 10 */,"JC\tw",[](I8085& cpu)
	{
		/* da JC nn */
		cpu.JumpIf([](const uint8_t flags) {
			return (flags & Flag::Carry) != 0;
		});
	}},
	{10,"IN\tb",[](I8085& cpu)
	{
		/* db IN n */
		cpu.Input(cpu.af.high);
	}},
	{9 /* or 18 */,"CC\tw",[](I8085& cpu)
	{
		/* dc CC nn */
		cpu.CallIf([](const uint8_t flags) {
			return (flags & Flag::Carry) != 0;
		});
	} },
	{7 /* or 10 */,"JNX5\tw",[](I8085& cpu)
	{
		/* dd *JNX5 nn */
		cpu.JumpIf([](const uint8_t flags) {
			return (flags & Flag::X5) == 0;
		});
	}},
	{ 7,"SBI\tb",[](I8085& cpu)
	{
		/* de SBI n */
		cpu.SubtractByteImmediateWithCarry(cpu.af.high);
	}},
	{12,"RST\t3",[](I8085& cpu)
	{
		/* df RST 3 */
		cpu.Call(0x18);
	} },
	{6 /* or 12 */,"RPO",[](I8085& cpu)
	{
		/* e0 RPO */
		cpu.ReturnIf([](const uint8_t flags) {
			return !(flags & Flag::Parity);
		});
	} },
	{10,"POP\tH",[](I8085& cpu)
	{
		/* e1 POP H */
		cpu.hl.word = cpu.PopWord();
	}},
	{7 /* or 10 */,"JPO\tw",[](I8085& cpu)
	{
		/* e2 JPO nn */
		cpu.JumpIf([](const uint8_t flags) {
			return !(flags & Flag::Parity);
		});
	}},
	{18,"XTHL",[](I8085& cpu)
	{
		/* e3 XTHL */
		cpu.ExchangeStack(cpu.hl.word);
	}},
	{9 /* or 18 */,"CPO\tw",[](I8085& cpu)
	{
		/* e4 CPO nn */
		cpu.CallIf([](const uint8_t flags) {
			return !(flags & Flag::Parity);
		});
	} },
	{12,"PUSH\tH",[](I8085& cpu)
	{
		/* e5 PUSH H */
		cpu.PushWord(cpu.hl.word);
	}},
	{7,"ANI\tb",[](I8085& cpu)
	{
		/* e6 ANI n */
		cpu.AndByteImmediate(cpu.af.high);
	}},
	{12,"RST\t4",[](I8085& cpu)
	{
		/* e7 RST 4 */
		cpu.Call(0x20);
	} },
	{6 /* or 12 */,"RPE",[](I8085& cpu)
	{
		/* e8 RPE */
		cpu.ReturnIf([](const uint8_t flags) {
			return (flags & Flag::Parity) != 0;
		});
	} },
	{6,"PCHL",[](I8085& cpu)
	{
		/* e9 PCHL */
		cpu.Jump(cpu.hl.word);
	}},
	{7 /* or 10 */,"JPE\tw",[](I8085& cpu)
	{
		/* ea JPE nn */
		cpu.JumpIf([](const uint8_t flags) {
			return (flags & Flag::Parity) != 0;
		});
	}},
	{4,"XCHG",[](I8085& cpu)
	{
		/* eb XCHG */
		Exchange(cpu.de.word, cpu.hl.word);
	}},
	{9 /* or 18 */,"CPE\tw",[](I8085& cpu)
	{
		/* ec CPE nn */
		cpu.CallIf([](const uint8_t flags) {
			return (flags & Flag::Parity) != 0;
		});
	} },
	{10,"LHLX",[](I8085& cpu)
	{
		/* ed *LHLX */
		cpu.LoadWord(cpu.hl.word, cpu.de.word);
	}},
	{7,"XRI\tb",[](I8085& cpu)
	{
		/* ee XRI n */
		cpu.XorByteImmediate(cpu.af.high);
	}},
	{12,"RST\t5",[](I8085& cpu)
	{
		/* ef RST 5 */
		cpu.Call(0x28);
	} },
	{6 /* or 12 */,"RP",[](I8085& cpu)
	{
		/* f0 RP */
		cpu.ReturnIf([](const uint8_t flags) {
			return !(flags & Flag::Sign);
		});
	} },
	{10,"POP\tPSW",[](I8085& cpu)
	{
		/* f1 POP PSW */
		cpu.af.word = cpu.PopWord();
	}},
	{7 /* or 10 */,"JP\tw",[](I8085& cpu)
	{
		/* f2 JP nn */
		cpu.JumpIf([](const uint8_t flags) {
			return !(flags & Flag::Sign);
		});
	}},
	{4,"DI",[](I8085& cpu)
	{
		/* f3 DI */
		cpu.interruptsDisabled = true;
	}},
	{9 /* or 18 */,"CP\tw",[](I8085& cpu)
	{
		/* f4 CP nn */
		cpu.CallIf([](const uint8_t flags) {
			return !(flags & Flag::Sign);
		});
	} },
	{12,"PUSH\tPSW",[](I8085& cpu)
	{
		/* f5 PUSH PSW */
		cpu.PushWord(cpu.af.word);
	} },
	{7,"ORI\tb",[](I8085& cpu)
	{
		/* f6 ORI n */
		cpu.OrByteImmediate(cpu.af.high);
	}},
	{12,"RST\t6",[](I8085& cpu)
	{
		/* f7 RST 6 */
		cpu.Call(0x30);
	} },
	{6 /* or 12 */,"RM",[](I8085& cpu)
	{
		/* f8 RM */
		cpu.ReturnIf([](const uint8_t flags) {
			return (flags & Flag::Sign) != 0;
		});
	} },
	{6,"SPHL",[](I8085& cpu)
	{
		/* f9 SPHL */
		cpu.sp = cpu.hl.word;
	}},
	{7 /* or 10 */,"JM\tw",[](I8085& cpu)
	{
		/* fa JM nn */
		cpu.JumpIf([](const uint8_t flags) {
			return (flags & Flag::Sign) != 0;
		});
	}},
	{ 4,"EI",[](I8085& cpu)
	{
		/* fb EI */
		cpu.interruptsDisabled = false;
	} },
	{9 /* or 18 */,"CM\tw",[](I8085& cpu)
	{
		/* fc CM nn */
		cpu.CallIf([](const uint8_t flags) {
			return (flags & Flag::Sign) != 0;
		});
	} },
	{ 7 /* or 10 */,"JX5\tw",[](I8085& cpu)
	{
		/* fd *JX5 nn */
		cpu.JumpIf([](const uint8_t flags) {
			return (flags & Flag::X5) != 0;
		});
	} },
	{ 7, "CPI\tb",[](I8085& cpu)
	{
		/* fe CPI n */
		cpu.CompareByteImmediate(cpu.af.high);
	}},
	{ 12,"RST\t7",[](I8085& cpu)
	{
		/* ff RST 7 */
		cpu.Call(0x38);
	} },
};
uint16_t I8085::FetchWord()
{
	auto low = FetchByte();
	auto high = FetchByte();
	return MakeWord(high, low);
}

void I8085::PushWord(uint16_t value)
{
	auto low = LowByte(value);
	auto high = HighByte(value);
	pMemorySpace->Write(--sp, high);
	pMemorySpace->Write(--sp, low);
}

uint16_t I8085::PopWord()
{
	auto low = pMemorySpace->Read(sp++);
	auto high = pMemorySpace->Read(sp++);
	return MakeWord(high, low);
}

void I8085::UpdateZeroSignParity(uint8_t value)
{
	if (value == 0) {
		SetFlag(Flag::Zero);
	}
	else {
		ClearFlag(Flag::Zero);
	}
	if ((value & 0x80) != 0) {
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
		SetFlag(Flag::Parity);
	}
	else {
		ClearFlag(Flag::Parity);
	}
}

void I8085::UpdateOverflowForAddition(uint8_t destination, uint8_t source, uint8_t result)
{
	if (((destination ^ result) & (source ^ result) & 0x80) != 0) {
		SetFlag(Flag::Overflow);
	}
	else {
		ClearFlag(Flag::Overflow);
	}
	if ((((destination & source) | (destination & result) | (source & result)) & 0x80) != 0) {
		SetFlag(Flag::X5);
	}
	else {
		ClearFlag(Flag::X5);
	}
}

void I8085::UpdateOverflowForSubtraction(uint8_t destination, uint8_t source, uint8_t result)
{
	if (((destination ^ source) & (destination ^ result) & 0x80) != 0) {
		SetFlag(Flag::Overflow);
	}
	else {
		ClearFlag(Flag::Overflow);
	}
	if ((((destination & source) | (destination & result) | (source & result)) & 0x80) != 0) {
		ClearFlag(Flag::X5);
	}
	else {
		SetFlag(Flag::X5);
	}
}

void I8085::MoveImmediateMemory(uint16_t address)
{
	uint8_t value;
	MoveImmediate(value);
	pMemorySpace->Write(address, value);
}

void I8085::AddByte(uint8_t& destination, uint8_t source)
{
	auto result = static_cast<uint16_t>(destination) + source;
	if (result & 0x100) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (((destination & 0x0f) + (source & 0x0f)) & 0x10) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	UpdateOverflowForAddition(destination, source, static_cast<uint8_t>(result));
	UpdateZeroSignParity(static_cast<uint8_t>(result));
	destination = static_cast<uint8_t>(result);
}

void I8085::AddByteMemory(uint8_t& destination, uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	AddByte(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::AddByteWithCarry(uint8_t& destination, uint8_t source)
{
	auto carry = (af.low & Flag::Carry) != 0 ? 1 : 0;
	auto result = static_cast<uint16_t>(destination) + source + carry;
	if (result & 0x100) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (((destination & 0x0f) + (source & 0x0f) + carry) & 0x10) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	UpdateOverflowForAddition(destination, source, static_cast<uint8_t>(result));
	UpdateZeroSignParity(static_cast<uint8_t>(result));
	destination = static_cast<uint8_t>(result);
}

void I8085::AddByteMemoryWithCarry(uint8_t& destination, uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	AddByteWithCarry(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::SubtractByte(uint8_t& destination, uint8_t source)
{
	auto result = static_cast<int16_t>(destination) - source;
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (static_cast<int16_t>(destination & 0x0f) - static_cast<int16_t>(source & 0x0f) < 0) {
		ClearFlag(Flag::HalfCarry);
	}
	else {
		SetFlag(Flag::HalfCarry);
	}
	UpdateOverflowForSubtraction(destination, source, static_cast<uint8_t>(result));
	UpdateZeroSignParity(static_cast<uint8_t>(result));
	destination = static_cast<uint8_t>(result);
}

void I8085::SubtractByteMemory(uint8_t& destination, const uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	SubtractByte(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::SubtractByteWithCarry(uint8_t& destination, const uint8_t source)
{
	auto carry = (af.low & Flag::Carry) != 0 ? 1 : 0;
	auto result = static_cast<int16_t>(destination) - source - carry;
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (static_cast<int16_t>(destination & 0x0f) - static_cast<int16_t>(source & 0x0f) - carry < 0) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	UpdateOverflowForSubtraction(destination, source, static_cast<uint8_t>(result));
	UpdateZeroSignParity(static_cast<uint8_t>(result));
	destination = static_cast<uint8_t>(result);
}

void I8085::SubtractByteMemoryWithCarry(uint8_t& destination, const uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	SubtractByteWithCarry(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::AndByte(uint8_t& destination, const uint8_t source)
{
	destination &= source;
	UpdateZeroSignParity(destination);
	ClearFlag(Flag::Carry);
	SetFlag(Flag::HalfCarry);
}

void I8085::AndByteMemory(uint8_t& destination, const uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	AndByte(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::XorByte(uint8_t& destination, const uint8_t source)
{
	destination ^= source;
	UpdateZeroSignParity(destination);
	ClearFlag(Flag::Carry);
	ClearFlag(Flag::HalfCarry);
}

void I8085::XorByteMemory(uint8_t& destination, const uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	XorByte(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::OrByte(uint8_t& destination, const uint8_t source)
{
	destination |= source;
	UpdateZeroSignParity(destination);
	ClearFlag(Flag::Carry);
	ClearFlag(Flag::HalfCarry);
}

void I8085::OrByteMemory(uint8_t& destination, const uint16_t address)
{
	auto value = pMemorySpace->Read(address);
	OrByte(destination, value);
	pMemorySpace->Write(address, value);
}

void I8085::CompareByte(const uint8_t destination, const uint8_t source)
{
	uint8_t dummy = destination;
	SubtractByte(dummy, source);
}

void I8085::CompareByteMemory(const uint8_t destination, const uint16_t source)
{
	auto value = pMemorySpace->Read(source);
	CompareByte(destination, value);
}

void I8085::IncreaseByte(uint8_t& byteRegister, const int offset)
{
	auto original = byteRegister;
	uint8_t byteOffset = static_cast<uint8_t>(offset);
	byteRegister += byteOffset;
	if (((original & 0x0f) + byteOffset) > 0x0f) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	UpdateZeroSignParity(byteRegister);
}

void I8085::IncreaseMemory(const uint16_t address, const int offset)
{
	auto value = pMemorySpace->Read(address);
	IncreaseByte(value, offset);
	pMemorySpace->Write(address, value);
}

void I8085::RotateLeftCircular(uint8_t& byteRegister)
{
	auto highBit = byteRegister & 0x80;
	byteRegister <<= 1;
	if (highBit) {
		byteRegister |= 1;
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
}

void I8085::RotateRightCircular(uint8_t& byteRegister)
{
	auto lowBit = byteRegister & 1;
	byteRegister >>= 1;
	if (lowBit) {
		byteRegister |= 0x80;
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
}

void I8085::RotateLeft(uint8_t& byteRegister)
{
	auto highBit = byteRegister & 0x80;
	byteRegister = (byteRegister << 1) | ((af.low & Flag::Carry) != 0 ? 1 : 0);
	if (highBit) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
}

void I8085::RotateRight(uint8_t& byteRegister)
{
	auto lowBit = byteRegister & 1;
	byteRegister = (byteRegister >> 1) | ((af.low & Flag::Carry) != 0 ? 0x80 : 0);
	if (lowBit) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
}

void I8085::DecimalAdjust(uint8_t& byteRegister)
{
	uint8_t correction = 0;
	bool carryFlag = false;
	if ((af.low & Flag::HalfCarry) != 0 || (byteRegister & 0x0f) > 9) {
		correction |= 0x06;
	}
	if ((af.low & Flag::Carry) != 0 || (byteRegister > 0x99) || (byteRegister > 0x8f && (byteRegister & 0x0f) > 9)) {
		correction |= 0x60;
		carryFlag = true;
	}
	if ((byteRegister & 0x0f) + (correction & 0x0f) >= 0x10) {
		SetFlag(Flag::HalfCarry);
	}
	else {
		ClearFlag(Flag::HalfCarry);
	}
	byteRegister += correction;
	if (carryFlag) {
		SetFlag(Flag::Carry);
	}
	UpdateZeroSignParity(byteRegister);
}

void I8085::LoadWordMemory(uint16_t& wordRegister)
{
	auto address = FetchWord();
	auto low = pMemorySpace->Read(address);
	auto high = pMemorySpace->Read(address + 1);
	wordRegister = MakeWord(high, low);
}

void I8085::LoadWord(uint16_t& wordRegister, const uint16_t address)
{
	auto low = pMemorySpace->Read(address);
	auto high = pMemorySpace->Read(address + 1);
	wordRegister = MakeWord(high, low);
}

void I8085::StoreWord(uint16_t address, uint16_t value)
{
	pMemorySpace->Write(address, LowByte(value));
	pMemorySpace->Write(address + 1, HighByte(value));
}

void I8085::AddWord(uint16_t& wordRegister, const uint16_t value)
{
	auto result = static_cast<uint32_t>(wordRegister) + value;
	if (result >= 0x10000) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	wordRegister = static_cast<uint16_t>(result);
}

void I8085::SubtractWord(uint16_t& wordRegister, uint16_t value)
{
	int32_t result = static_cast<int32_t>(wordRegister) - value;
	if (result < 0) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	wordRegister = static_cast<uint16_t>(result);
}

void I8085::IncreaseWord(uint16_t& wordRegister, const int offset)
{
	int32_t result = static_cast<int32_t>(wordRegister) + offset;
	if (result < 0 || result >= 0x10000) {
		SetFlag(Flag::X5);
	}
	else {
		ClearFlag(Flag::X5);
	}
	wordRegister = static_cast<uint16_t>(result);
}

void I8085::AddWordByteConst(uint16_t& destination, const uint16_t source)
{
	auto byteValue = FetchByte();
	destination = source + byteValue;
}

void I8085::ShiftWordRightArithmetic(uint16_t& wordRegister)
{
	auto lowBit = wordRegister & 1;
	wordRegister = (wordRegister >> 1) | (wordRegister & 0x8000);
	if (lowBit) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
}

void I8085::RotateWordLeft(uint16_t& wordRegister)
{
	auto highBit = wordRegister & 0x8000;
	wordRegister = (wordRegister << 1) | ((af.low & Flag::Carry) != 0 ? 1 : 0);
	if (highBit) {
		SetFlag(Flag::Carry);
	}
	else {
		ClearFlag(Flag::Carry);
	}
	if (wordRegister == 0) {
		SetFlag(Flag::Zero);
	}
	else {
		ClearFlag(Flag::Zero);
	}
	if ((wordRegister & 0x8000) != 0) {
		SetFlag(Flag::Sign);
	}
	else {
		ClearFlag(Flag::Sign);
	}
	if (highBit != (wordRegister & 0x8000)) {
		SetFlag(Flag::Overflow);
	}
	else {
		ClearFlag(Flag::Overflow);
	}
	auto parity = 0;
	for (int i = 0; i < 16; ++i) {
		if (wordRegister & (1 << i)) {
			++parity;
		}
	}
	if ((parity & 1) == 0) {
		SetFlag(Flag::Parity);
	}
	else {
		ClearFlag(Flag::Parity);
	}
}

void I8085::Exchange(uint16_t& word1, uint16_t& word2)
{
	auto temp = word1;
	word1 = word2;
	word2 = temp;
}

void I8085::ExchangeStack(uint16_t& wordRegister)
{
	auto temp = PopWord();
	PushWord(wordRegister);
	wordRegister = temp;
}

void I8085::WriteInterruptBits(uint8_t byteRegister)
{
	if (byteRegister & InterruptBit::SetRst) {
		interruptBits = (interruptBits & ~InterruptBit::MaskRst) | (byteRegister & InterruptBit::MaskRst);
	}
	if (byteRegister & InterruptBit::ClearRst75) {
		interruptBits &= ~InterruptBit::RequestRst75;
	}
	if (byteRegister & InterruptBit::SetSod) {
		interruptBits = (interruptBits & ~InterruptBit::Sod) | (byteRegister & InterruptBit::Sod);
	}
}

void I8085::Jump(const uint16_t address) {
	pc = address;
	//FetchInstruction();
}

void I8085::Jump()
{
	uint16_t address = FetchWord();
	Jump(address);
}

void I8085::JumpIf(const std::function<bool(uint8_t)>& condition)
{
	if (condition(af.low)) {
		clockCountToExecute += 3;
		pNextInstruction = &Instructions[0xc3]; // JMP
	}
	else {
		auto aaa = 111;
		FetchWord();// skip the address operand
	}
}

void I8085::Call(const uint16_t address)
{
	PushWord(pc);
	pc = address;
	//FetchInstruction();
}

void I8085::CallIf(const std::function<bool(uint8_t)>& condition)
{
	if (condition(af.low)) {
		clockCountToExecute += 9;
		pNextInstruction = &Instructions[0xcd]; // CALL
	}
	else {
		FetchWord();// skip the address operand
	}
}

void I8085::Return()
{
	pc = PopWord();
	//FetchInstruction();
}

void I8085::ReturnIf(const std::function<bool(uint8_t)>& condition)
{
	if (condition(af.low)) {
		clockCountToExecute += 6;
		pNextInstruction = &Instructions[0xc9]; // RET
	}
}

void I8085::RestartIfOverflow()
{
	static const Instruction Rst8 = { 12,"RST\t8",[](I8085& cpu)
	{
		cpu.Call(0x40);
	} };

	if ((af.low & Flag::Overflow) != 0) {
		clockCountToExecute += 6;
		pNextInstruction = &Rst8;
	}
}

void I8085::Input(uint8_t& byteRegister)
{
	auto address = FetchByte();
	byteRegister = pIoSpace->Read(address);
}

void I8085::Output(const uint8_t byteRegister)
{
	auto address = FetchByte();
	pIoSpace->Write(address, byteRegister);
}


void I8085::Reset()
{
	static_assert(std::size(Instructions) == 256, "Instructions array must have exactly 256 elements (0x00-0xFF)");
	af.word = 0;
	bc.word = 0;
	de.word = 0;
	hl.word = 0;
	sp = 0;
	pc = 0;
	interruptsDisabled = false;
	interruptBits = InterruptBit::MaskRst | InterruptBit::MaskIntr;
	halted = false;
	FetchInstruction();
}

void I8085::OnClock(uint32_t time)
{
	if (--clockCountToExecute == 0) {
		auto pInstruction = pNextInstruction;

		//char buffer[256];
		//sprintf_s(buffer, sizeof(buffer), "%04X: %s %02X\n", currentInstructionPc, pInstruction->mnemonic, af.high);
		//OutputDebugString(buffer);

		pInstruction->execute(*this);
		af.low &= 0xd5;
		af.low |= 0x02;
		if (pNextInstruction == pInstruction) {
			FetchInstruction();
		}
	}
}

void I8085::FetchInstruction()
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
}
