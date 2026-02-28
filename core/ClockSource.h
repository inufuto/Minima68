#pragma once
#include <cstdint>
#include <vector>

class ClockDestination
{
public:
	virtual void OnClock(uint32_t time) = 0;
	virtual ~ClockDestination() = default;
};

class ClockSource 
{
private:
	std::vector<ClockDestination*> destinations;
public:
	void AddDestination(ClockDestination* destination) { destinations.push_back(destination); }
	void OnClock(uint32_t time);
	virtual ~ClockSource() = default;
};