#include "EmulatorCore.h"

uint8_t EmulatorCore::bytes[0x10000] = {
	0xc3, 0x00, 0x01, // JMP 0x0100
};