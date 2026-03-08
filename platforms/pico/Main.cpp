#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"

#include "Minima68Pico.h"
#include "Ntsc.h"

int main()
{
    stdio_init_all();
    InitNtsc();
    emulator.Run();
}
