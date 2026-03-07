#pragma once

#include "AbtractEmulator.h"
#include "ClockSource.h"

inline uint16_t MakeWord(uint8_t high, uint8_t low) { return high << 8 | low; }
inline uint8_t HighByte(uint16_t word) { return word >> 8; }
inline uint8_t LowByte(uint16_t word) { return word & 0xff; }

struct AssemblyElement
{
	int size;
	std::string mnemonic;
	std::string operand;
};

class Debugger
{
public:
	virtual void Pause() = 0;
	virtual bool PauseRequired(const class Cpu* pCpu, uint16_t address);
};

class Cpu : public BreakpointHolder, public ClockDestination, public RegisterHolder
{
private:
	Debugger* pDebugger;
protected:
	auto GetDebugger() const { return pDebugger; }
public:
	explicit Cpu(class Debugger* pDebugger) : pDebugger(pDebugger) {}
	virtual ~Cpu() = default;
	virtual void Reset() = 0;
	virtual uint16_t CurrentInstructionAddress() const = 0;
	virtual void BuildAssemblyElement(uint16_t address, AssemblyElement* pElement) const = 0;
};

inline bool Debugger::PauseRequired(const Cpu* pCpu, uint16_t address) {
	return pCpu->ContainsBreakpoint(address);
}
