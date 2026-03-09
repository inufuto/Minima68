#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include "Config.h"
#include "SoundChannel.h"
#include "Minima68Pico.h"
#include "MemoryMap.h"
#include "Sound.h"

static constexpr auto ReferenceToneFrequency = 440.0;
static constexpr auto PwmWrap = 0x100;
static constexpr auto PwmDivision =
    (Config::SystemClock * 1000.0) /
    (ReferenceToneFrequency * ToneSampleCount * PwmWrap);
static constexpr auto PwmSampleRate =
    (Config::SystemClock * 1000.0) / (PwmDivision * PwmWrap);
static constexpr int32_t PwmSampleRateInt = static_cast<int32_t>(PwmSampleRate + 0.5);

void ToneChannel::SetFrequency(uint16_t frequency)
{
    phaseDelta = static_cast<uint32_t>(frequency) * ToneSampleCount;
}

uint8_t ToneChannel::Sample()
{
    auto sample = pSamples[sampleIndex] * volume / MaxVolume;

    phaseError -= static_cast<int32_t>(phaseDelta);
    while (phaseError < 0) {
        phaseError += PwmSampleRateInt;
        ++sampleIndex;
        if (sampleIndex >= ToneSampleCount) {
            sampleIndex = 0;
        }
    }

    return sample;
}


ToneChannel toneChannels[ToneChannelCount];

static void PwmHandler()
{
    auto pwmSlice = pwm_gpio_to_slice_num(Config::Gpio::Sound);
    pwm_clear_irq(pwmSlice);

    uint16_t sum = 0;
    for (auto& channel : toneChannels) {
        sum += channel.Sample();
    }
    auto level = sum / (ToneChannelCount + 1);
    pwm_set_gpio_level(Config::Gpio::Sound, level);
}


void InitSound()
{
    // {
    //     auto pSourceSamples = emulator.Ram() + ShortWaveAddress;
    //     for (auto& channel : toneChannels) {
    //         channel.Samples(pSourceSamples);
    //         pSourceSamples += ToneSampleCount;
    //     }
    // }
    // toneChannels[0].SetFrequency(440);
    
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