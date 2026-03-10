#include <string>

#include "Minima68Win.h"
#include "SoundChannel.h"

const uint8_t PianoWave[] = {
	254,238,204,172,150,128,104,84,
	74,65,50,36,32,33,25,9,
	0,9,25,33,32,36,50,65,
	74,84,104,128,150,172,204,238,
};
const uint8_t Lead2Wave[] = {
	203,160,75,22,17,23,17,10,
	11,10,6,4,4,3,0,0,
	2,0,0,3,4,4,6,10,
	11,10,17,23,17,22,75,160,
};


Minima68Win::Minima68Win(PrimaryClock::Owner* pOwner): 
	Minima68(this), primaryClock(pOwner, PrimaryClockFrequency), vsync(this)
{
	videoClockSource.AddDestination(&vsync);
	primaryClock.AddDestination(&Cpu());
	primaryClock.AddDestination(&videoClockSource);
}

void Minima68Win::Start()
{
	mode = Normal;
	Reset();
	SoundThread.Start(this);
	primaryClock.Start();
}

void Minima68Win::Stop()
{
	SoundThread.Stop();
	primaryClock.Stop();
}

bool Minima68Win::PauseRequired(const ::Cpu* pCpu, uint16_t address)
{
	switch (mode) {
	case Step:
		if (address != lastInstructionAddress) {
			mode = Normal;
			return true;
		}
		break;
	case Next:
		if (address == nextProgramCounter) {
			mode = Normal;
			return true;
		}
		break;
	}
	return Debugger::PauseRequired(pCpu, address);
}

void Minima68Win::ExecuteStep()
{
	lastInstructionAddress = Cpu().CurrentInstructionAddress();
	mode = Step;
	Resume();
}

void Minima68Win::ExecuteToNext()
{
	nextProgramCounter = Cpu().CurrentInstructionAddress() + Cpu().CurrentInstructionSize();
	mode = Next;
	Resume();
}

void Minima68Win::SetColor(int index, uint8_t r, uint8_t g, uint8_t b) {
	assert(index >= 0 && index < ColorCount);
	colors[index] = { b, g, r, 0xff };
}

void Minima68Win::SetToneSample(int index, const uint8_t* pSample)
{
	assert(index >= 0 && index < ToneSampleCount);
	ToneChannels[index].SetSourceSamples(pSample);
}

void Minima68Win::SetToneFrequency(int index, uint16_t frequency)
{
	assert(index >= 0 && index < ToneSampleCount);
	ToneChannels[index].SetFrequency(frequency);
}

void Minima68Win::SetToneVolume(int index, uint8_t volume)
{
	assert(index >= 0 && index < ToneSampleCount);
	ToneChannels[index].SetVolume(volume);
}
