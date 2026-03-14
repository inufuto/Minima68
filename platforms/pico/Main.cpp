#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/dma.h>
#include <tusb.h>
#include <hid.h>

#include "Minima68Pico.h"
#include "Ntsc.h"
#include "SoundChannel.h"
#include "SdCardPetitFatFs.h"
#include "Input.h"

extern void RunLauncher();

int main()
{
    stdio_init_all();
    tusb_init();
    InitSdCardPetitFatFs();
    InitNtsc();
    InitSound();
    InitInput();
    RunLauncher();
    emulator.Run();
}
