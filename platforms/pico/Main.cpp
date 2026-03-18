#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <hardware/dma.h>
#include <tusb.h>
#include <hid.h>

#include "Minima68Pico.h"
#include "Ntsc.h"
#include "SoundChannel.h"
#include "SdCardPetitFatFs.h"
#include "Input.h"

extern void RunLauncher();

void main1()
{
    InitNtsc();
    InitSound();
    while (true);
}

int main()
{
    stdio_init_all();
    tusb_init();
    InitSdCardPetitFatFs();
    InitInput();
    multicore_launch_core1(main1);
    RunLauncher();
    emulator.Run();
}
