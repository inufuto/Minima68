#pragma once
#include <cstdint>

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

struct PairRegister
{
	uint8_t high;
	uint8_t low;

	uint16_t Word() const { return MakeWord(high, low); }
	void Word(uint16_t value)
	{
		high = HighByte(value);
		low = LowByte(value);
	}
};
