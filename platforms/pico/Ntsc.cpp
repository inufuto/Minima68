#include <stdint.h>
#include <cstring>
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/structs/pwm.h"
#include "../../core/Video.h"
#include "Minima68Pico.h"
#include "Config.h"
#include "MemoryMap.h"
#include "Color.h"

static volatile uint32_t TimerCount = 0;
static void SoundHandler() {}

static constexpr auto DmaChannelCount = 2;
static constexpr auto SamplesPerRaster = 908; // 227 * 4
static constexpr auto PwmCycle = Config::SystemClock * 1000 / 14318181;
static constexpr auto RasterCount = 262;
static constexpr auto VSyncRasterCount = 10;
static constexpr auto BlankingRasterCount = 232 - YResolution;
static constexpr auto HSyncLength = 68; // Approximately 4.7μsec
static constexpr auto HStartPosition = HSyncLength + 120;

static Color colors[ColorCount];
static int dmaChannels[DmaChannelCount];
static uint16_t dmaBuffer[DmaChannelCount][SamplesPerRaster] __attribute__ ((aligned (4)));
static volatile uint16_t currentRaster;
static volatile int currentY;
static volatile uint8_t* pTileRow;
static volatile int yMod;
static volatile int xModLeft;

const auto TileMap = emulator.Ram() + TileMapAddress;
const auto TilePattern = emulator.Ram() + TilePatternAddress;
const auto SpriteAttributes = emulator.Ram() + SpriteAttributeAddress;
const auto SpritePattern = emulator.Ram() + SpritePatternAddress;

static void MakeDmaBuffer(uint16_t* pBuffer, uint16_t raster)
{
    auto p = pBuffer;
    if (raster < 2) {
        // VSync
        for (auto i = 0; i < SamplesPerRaster - HSyncLength; ++i) {
            *p++ = 0;
        }
        while (p < pBuffer + SamplesPerRaster) {
            *p++ = 2;
        } 
    }
    else if(raster == VSyncRasterCount || raster == VSyncRasterCount + 1) {
        // HSync & Color Burst
        for (auto i = 0; i < HSyncLength; ++i) {
            *p++ = 0;
        }
        for (auto i = 0; i < 8; ++i) {
            *p++ = 2;
        }
        for (auto i = 0; i < 9; ++i) {
            *p++ = 2;
            *p++ = 1;
            *p++ = 2;
            *p++ = 3;
        }
        while (p < pBuffer + SamplesPerRaster) {
            *p++ = 2;
        } 
    }
    else if (
        raster >= VSyncRasterCount + BlankingRasterCount &&
        raster < VSyncRasterCount + BlankingRasterCount + YResolution
    ) {
        static uint8_t lineBuffer[XResolution];
        if (raster == VSyncRasterCount + BlankingRasterCount) {
            currentY = 0;
            pTileRow = TileMap;
            yMod = xModLeft = 0;
        }
        else if (raster == VSyncRasterCount + BlankingRasterCount + TileHeight * StatusAreaHeight) {
            auto scrollY = emulator.ScrollY();
            auto scrollX = emulator.ScrollX();
            pTileRow = TileMap + VramWidth * StatusAreaHeight +
                 (scrollY / TileHeight) * VramWidth +
                  (scrollX / TileWidth);
            yMod = scrollY % TileHeight;
            xModLeft = scrollX % TileWidth;
        }
        {
            auto pLine = lineBuffer;
            auto pTile = pTileRow;
            auto xMod = xModLeft;
            auto tile = *pTile++;
            auto pPattern = TilePattern + 
                (static_cast<uint16_t>(tile) * TilePatternSize) +
                (yMod * TileWidthInBytes) +
                xMod / DotsPerByte;
            auto patternByte = *pPattern++;
            for (auto x = 0; x < XResolution; ++x) {
                if ((xMod % DotsPerByte) == 0) {
                    *pLine++ = patternByte >> 4;
                }
                else {
                    *pLine++ = patternByte & 0x0f;
                }
                if (++xMod >= TileWidth) {
                    xMod = 0;
                    tile = *pTile++;
                    pPattern = TilePattern +
                        (static_cast<uint16_t>(tile) * TilePatternSize) +
                        (yMod * TileWidthInBytes);
                }
                if ((xMod % DotsPerByte) == 0) {
                    patternByte = *pPattern++;
                }
            }
            if (++yMod >= TileHeight) {
                pTileRow += VramWidth;
                yMod = 0;
            }
        }
        {
            auto horizontalCount = 0;
            auto pSprite = SpriteAttributes + SpriteCount;
            for (auto i = 0; i < SpriteCount; ++i) {
                --pSprite;
                uint8_t yOffset = currentY - pSprite[0]; // y
                if (yOffset < SpriteHeight) {
                    uint8_t x = pSprite[1]; // x
                    auto pPattern = SpritePattern +
                        (static_cast<uint16_t>(pSprite[2]) << 6); // pattern
                    pPattern += yOffset << 2;
                    for (auto j = 0; j < SpriteWidth / 2; ++j) {
                        auto b = *pPattern; //0x44; //
                        if (x < XResolution) {
                            auto dot = b >> 4;
                            if (dot != 0) {
                                lineBuffer[x] = dot;
                            }
                        }
                        ++x;
                        if (x < XResolution) {
                            auto dot = b & 0x0f;
                            if (dot != 0) {
                                lineBuffer[x] = dot;
                            }
                        }
                        ++x;
                        ++pPattern;
                    }
                    if (++horizontalCount >= MaxHorizontalSpriteCount) {
                        break;
                    }
                }
            }
        }
        {
            auto pLine = lineBuffer;
            auto mod = 0;
            p += HStartPosition;
            for (auto i = 0; i < XResolution / 4; ++i) {
                const auto& color0 = colors[*pLine++];
                *p++ = color0.Values()[0];
                *p++ = color0.Values()[1];
                *p++ = color0.Values()[2];
                *p++ = color0.Values()[3];
                *p++ = color0.Values()[0];
                const auto& color1 = colors[*pLine++];
                *p++ = color1.Values()[1];
                *p++ = color1.Values()[2];
                *p++ = color1.Values()[3];
                *p++ = color1.Values()[0];
                *p++ = color1.Values()[1];
                const auto& color2 = colors[*pLine++];
                *p++ = color2.Values()[2];
                *p++ = color2.Values()[3];
                *p++ = color2.Values()[0];
                *p++ = color2.Values()[1];
                *p++ = color2.Values()[2];
                const auto& color3 = colors[*pLine++];
                *p++ = color3.Values()[3];
                *p++ = color3.Values()[0];
                *p++ = color3.Values()[1];
                *p++ = color3.Values()[2];
                *p++ = color3.Values()[3];
            }
        }
        ++currentY;
    }
    else if (
        raster == VSyncRasterCount + BlankingRasterCount + YResolution ||
        raster == VSyncRasterCount + BlankingRasterCount + YResolution + 1
    ) {
        p += HStartPosition;
        for (auto i = 0; i < XResolution * 5; ++i) {
            *p++ = 2;
        }
    }
}


inline uint32_t InterruptBit(int channel)
{
    return 1u << channel; 
}

static void Handler()
{
    volatile auto status = dma_hw->ints0;
    dma_hw->ints0 = status; // Clear IRQ

    for (auto i = 0; i < DmaChannelCount; ++i) {
        if ((status & InterruptBit(dmaChannels[i])) != 0) {
            MakeDmaBuffer(dmaBuffer[i], currentRaster);
		    dma_channel_set_read_addr(dmaChannels[i], dmaBuffer[i], false);
        }
    }
    if (++currentRaster == RasterCount) {
        currentRaster = 0;
        ++TimerCount;
        SoundHandler();
    }
}

static void InitializePwmDma()
{
    gpio_set_function(Config::Gpio::Video, GPIO_FUNC_PWM);
    auto pwmSlice = pwm_gpio_to_slice_num(Config::Gpio::Video);

    auto pwmConfig = pwm_get_default_config();
    pwm_config_set_clkdiv(&pwmConfig, 1);

    pwm_init(pwmSlice, &pwmConfig, true);
    pwm_set_wrap(pwmSlice, PwmCycle - 1);

    auto pOut = reinterpret_cast<io_rw_16 *>(&pwm_hw->slice[pwmSlice].cc) + 1;
    for (auto i = 0; i < DmaChannelCount; ++i)
    {
        dmaChannels[i] = dma_claim_unused_channel(true);
    }
    uint32_t mask = 0;
    for (auto i = 0; i < DmaChannelCount; ++i)
    {
        auto dmaChannelConfig = dma_channel_get_default_config(dmaChannels[i]);
        channel_config_set_transfer_data_size(&dmaChannelConfig, DMA_SIZE_16);
        channel_config_set_read_increment(&dmaChannelConfig, true);
        channel_config_set_write_increment(&dmaChannelConfig, false);
        channel_config_set_dreq(&dmaChannelConfig, DREQ_PWM_WRAP0 + pwmSlice);
        auto nextChannel = dmaChannels[(i + 1) % DmaChannelCount];
        channel_config_set_chain_to(&dmaChannelConfig, nextChannel);
        dma_channel_configure(
            dmaChannels[i],
            &dmaChannelConfig,
            pOut,
            dmaBuffer[i],
            SamplesPerRaster,
            false);
        memset(dmaBuffer[i],0 , SamplesPerRaster * sizeof(uint16_t));
        MakeDmaBuffer(dmaBuffer[i], i);
        mask |= InterruptBit(dmaChannels[i]);
    }
    dma_set_irq0_channel_mask_enabled(mask ,true);
    irq_set_priority(DMA_IRQ_0, 0x20);
	irq_set_exclusive_handler(DMA_IRQ_0, Handler);
	irq_set_enabled(DMA_IRQ_0, true);
    currentRaster = 0;
	dma_channel_start(dmaChannels[0]);
}

void InitNtsc()
{
    // for (auto& sprite : SpriteAttributes) {
    //     sprite.y = YResolution;
    // }
    set_sys_clock_khz(Config::SystemClock, true);
    InitializePwmDma();
}

void SetColor(int index, uint8_t r, uint8_t g, uint8_t b)
{
    assert(index < ColorCount);
    colors[index].SetRgb(r, g, b);
}