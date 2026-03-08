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

class SecondaryClockSource : public ClockSource, public ClockDestination
{
private:
	double frequencyRatio;
public:
	explicit SecondaryClockSource(double frequencyRatio) : frequencyRatio(frequencyRatio) {}
	void OnClock(uint32_t time) override {
		static double accumulatedTime = 0;
		accumulatedTime += frequencyRatio;
		while (accumulatedTime >= 1.0) {
			ClockSource::OnClock(time);
			accumulatedTime -= 1.0;
		}
	}
};