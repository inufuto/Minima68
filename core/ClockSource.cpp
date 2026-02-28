#include "ClockSource.h"

void ClockSource::OnClock(uint32_t time)
{
	for (auto pDestination : destinations) {
		pDestination->OnClock(time);
	}
}
