#pragma once

#include "PrimaryClock.h"
#include "../../../core/Minima68.h"
#include "../../../core/Video.h"
#include "../Audio/WindowsAudioClient.h"

class Minima68Win : public Minima68, public Debugger
{
private:
	static constexpr double ScreenRefreshRate = 60;
	PrimaryClock primaryClock;
private:
	enum Mode
	{
		Normal, Step, Next
	} mode;
	uint16_t lastInstructionAddress;
	uint16_t nextProgramCounter;
	union Color
	{
		struct {
			uint8_t b, g, r, a;
		};
		uint32_t dword;
	} colors[ColorCount];
	class VideoClockSource : public SecondaryClockSource
	{
	public:
		explicit VideoClockSource() : SecondaryClockSource(ScreenRefreshRate / PrimaryClockFrequency) {}
	} videoClockSource;
	class VSync : public ClockDestination, public Uncopyable
	{
	private:
		Minima68Win* pOwner;
	public:
		explicit VSync(Minima68Win* pOwner) : pOwner(pOwner) {}
		void OnClock(uint32_t time) override { pOwner->MakeInterrupt(); }
	} vsync;
public:
	explicit Minima68Win(PrimaryClock::Owner* pOwner);
	const auto& ClockSource() const { return primaryClock; }
	auto& ClockSource() { return primaryClock; }
	void Start();
	void Stop();
	void Pause() override { primaryClock.Pause(); }
	void Resume() { primaryClock.Resume(); }
	bool Paused() const { return primaryClock.Paused(); }
	bool PauseRequired(const ::Cpu* pCpu, uint16_t address) override;
	void ExecuteStep();
	void ExecuteToNext();

	void SetColor(int index, uint8_t r, uint8_t g, uint8_t b) override;
	uint32_t ColorAt(int index) const { assert(index >= 0 && index < ColorCount); return colors[index].dword; }

	void SetToneSample(int index, const uint8_t* pSample) override;
	void SetToneFrequency(int index, uint16_t frequency) override;
	void SetToneVolume(int index, uint8_t volume) override;
};
