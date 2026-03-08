
#include "../core/ClockSource.h"

class MasterClock : public ClockSource
{
private:
    double targetFrequency;
    int32_t time;
public:
    MasterClock(double targetFrequency) : targetFrequency(targetFrequency){}
    void Run();
};