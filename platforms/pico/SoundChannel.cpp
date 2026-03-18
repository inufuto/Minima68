#include <algorithm>
#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include "Config.h"
#include "SoundChannel.h"
#include "Minima68Pico.h"
#include "MemoryMap.h"
#include "Sound.h"

static constexpr auto PwmWrap = 0x100;
static constexpr uint32_t PwmSampleRateHz = 22050;
static constexpr uint16_t DefaultEffectFrequencyHz = 4000;
static constexpr auto PwmSampleRate = static_cast<double>(PwmSampleRateHz);  // Standard audio sample rate
static constexpr auto PwmDivision =
    (Config::SystemClock * 1000.0) /
    (PwmSampleRate * PwmWrap);

void ToneChannel::SetFrequency(uint16_t frequency)
{
    phaseDelta = (static_cast<uint64_t>(frequency) << 32) / PwmSampleRateHz;
}

void ToneChannel::SetVolume(uint8_t volume)
{
    this->volume = std::min(static_cast<int>(volume), MaxVolume);
}

uint8_t __not_in_flash_func(ToneChannel::Sample)()
{
    if (volume == 0 || phaseDelta == 0) {
        return 0;
    }

    phase += phaseDelta;

    // Top bit indicates high/low half-cycle of a 50% duty square wave.
    uint8_t sample = ((phase & 0x80000000u) != 0) ? 255 : 0;
    return sample * volume / MaxVolume;
}

void EffectChannel::SetVolume(uint8_t volume)
{
    this->volume = std::min(static_cast<int>(volume), MaxVolume);
    if (this->volume > 0 && phaseDelta == 0) {
        SetFrequency(DefaultEffectFrequencyHz);
    }
    if ((volume & 0x80) != 0) {
        phase = 0;
        noiseState = 1;  // Reset LFSR
    }
}

void EffectChannel::SetFrequency(uint16_t frequency)
{
    phaseDelta = (static_cast<uint64_t>(frequency) << 32) / PwmSampleRateHz;
}

void EffectChannel::SetRate(uint8_t rate)
{
    (void)rate;
}

uint8_t __not_in_flash_func(EffectChannel::Sample)()
{
    if (volume > 0 && phaseDelta != 0) {
        const uint32_t previousPhase = phase;
        phase += phaseDelta;

        // Advance LFSR at configured frequency.
        if (phase < previousPhase) {
            const uint32_t lsb = noiseState & 1;
            noiseState >>= 1;
            if (lsb != 0) {
                noiseState ^= 0xB800;  // Feedback polynomial
            }
        }

        const uint8_t noise = (noiseState & 1) != 0 ? 255 : 0;
        return noise * volume / MaxVolume;
    }
    return 0;
}


ToneChannel toneChannels[ToneChannelCount];
EffectChannel effectChannel;

static void __not_in_flash_func(PwmHandler)()
{
    auto pwmSlice = pwm_gpio_to_slice_num(Config::Gpio::Sound);
    pwm_clear_irq(pwmSlice);

    uint16_t sum = 0;
    for (auto& channel : toneChannels) {
        sum += channel.Sample();
    }
    sum += effectChannel.Sample();
    auto level = sum / (ToneChannelCount + 1);
    pwm_set_gpio_level(Config::Gpio::Sound, level);
}


void InitSound()
{
    gpio_set_function(Config::Gpio::Sound, GPIO_FUNC_PWM);
    auto pwmSlice = pwm_gpio_to_slice_num(Config::Gpio::Sound);

    auto pwmConfig = pwm_get_default_config();
    pwm_init(pwmSlice, &pwmConfig, true);
    pwm_set_clkdiv(pwmSlice, PwmDivision);
    pwm_set_wrap(pwmSlice, PwmWrap - 1);

    pwm_set_gpio_level(Config::Gpio::Sound, 0);

    pwm_clear_irq(pwmSlice);
    pwm_set_irq_enabled(pwmSlice, true);
    irq_set_priority(PWM_IRQ_WRAP, 0x80);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, PwmHandler);
    irq_set_enabled(PWM_IRQ_WRAP, true);
}