#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/dma.h>
#include <tusb.h>
#include <hid.h>

#include "Minima68Pico.h"
#include "Ntsc.h"
#include "SoundChannel.h"
#include "SdCardPetitFatFs.h"
#include "Joystick.h"

extern void RunLauncher();

int main()
{
    stdio_init_all();
    tusb_init();
    InitButtons();
    InitSdCardPetitFatFs();
    InitNtsc();
    InitSound();
    RunLauncher();
    emulator.Run();
}
