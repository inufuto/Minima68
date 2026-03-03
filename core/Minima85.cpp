#include "Minima85.h"

#include <cassert>

const ::MemorySpace* Minima85::MemorySpaceAt(int index) const
{
	assert(index >= 0 && index < 2);
	if (index == 1) {
		return &port;
	}
	return &memory;
}
