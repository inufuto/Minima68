
#include "../core/ClockSource.h"

class PrimaryClock : public ClockSource
{
private:
    double targetFrequency;
    int32_t time;
public:
    PrimaryClock(double targetFrequency) : targetFrequency(targetFrequency){}
    void Run();
};