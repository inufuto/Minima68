#include "Z80Cpu.h"

const Z80Cpu::Instruction Z80Cpu::Instructions[] = {
	{ 4, [](Z80Cpu&)
	{
		/* NOP */
	} },
	{ 4, [](Z80Cpu& cpu)
	{
		/* LD BC, nn */
		cpu.LoadConstWord(cpu.CurrentExchangeableRegisters().bc);
	} },
};

void Z80Cpu::LoadConstWord(PairRegister& pairRegister)
{
	pairRegister.low = FetchMemory();
	pairRegister.high = FetchMemory();
}

void Z80Cpu::Reset()
{
	exchangeableRegisterIndex = 0;
	afIndex = 0;
	for (auto& exchangeableRegisters : exchangeableRegisterses) {
		exchangeableRegisters.bc.Word(0);
		exchangeableRegisters.de.Word(0);
		exchangeableRegisters.hl.Word(0);
	}
	for (auto& af : afs) {
		af.Word(0);
	}
	for (unsigned char& iff : iffs) {
		iff = 0;
	}
	ix.Word(0);
	iy.Word(0);
	sp = 0;
	pc = 0;
	FetchInstruction();
}

void Z80Cpu::FetchInstruction()
{
	uint8_t opcode = FetchMemory();
	switch (opcode) {
	case 0xcb:
		pNextInstruction = &InstructionsAfterCb[FetchMemory()];
		break;
	case 0xed:
		pNextInstruction = &InstructionsAfterEd[FetchMemory()];
		break;
	case 0xdd:
		pNextInstruction = &InstructionsAfterDd[FetchMemory()];
		break;
	case 0xfd:
		pNextInstruction = &InstructionsAfterFd[FetchMemory()];
		break;
	default:
		pNextInstruction = &Instructions[opcode];
	}
	clockCountToExecute = pNextInstruction->clockCount;
}

void Z80Cpu::OnClock(uint32_t time)
{
	if (--clockCountToExecute == 0) {
		pNextInstruction->execute(*this);
		FetchInstruction();
	}
}
