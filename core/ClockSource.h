#pragma once
#include <cstdint>
#include <vector>

#include "Uncopyable.h"

class ClockDestination
{
public:
	virtual void OnClock(uint32_t time) = 0;
};

class ClockSource : public Uncopyable
{
private:
	std::vector<ClockDestination*> destinations;
public:
	void AddDestination(ClockDestination* destination) { destinations.push_back(destination); }
	void OnClock(uint32_t time);
	virtual ~ClockSource() = default;
};