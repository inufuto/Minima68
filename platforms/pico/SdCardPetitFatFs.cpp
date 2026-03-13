#include "SdCardPetitFatFs.h"

#include <cstring>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

#include "Config.h"

extern "C" {
#include "diskio.h"
#include "pff.h"
}

namespace {
spi_inst_t* const kSpi = spi0;
constexpr uint32_t kInitBaudrateHz = 400000;
constexpr uint32_t kTransferBaudrateHz = 12000000;
constexpr uint8_t kCmd0 = 0;
constexpr uint8_t kCmd8 = 8;
constexpr uint8_t kCmd16 = 16;
constexpr uint8_t kCmd17 = 17;
constexpr uint8_t kCmd55 = 55;
constexpr uint8_t kCmd58 = 58;
constexpr uint8_t kAcmd41 = 41;

constexpr uint8_t kDataToken = 0xFE;
constexpr int kCmdRetries = 100;
constexpr int kInitRetries = 200;
constexpr int kReadTokenRetries = 10000;

bool g_sdReady = false;
bool g_highCapacity = false;
FATFS g_fs;

inline void SelectCard() {
    gpio_put(Config::Gpio::SdCs, 0);
}

inline void DeselectCard() {
    gpio_put(Config::Gpio::SdCs, 1);
    uint8_t dummy = 0xFF;
    spi_write_blocking(kSpi, &dummy, 1);
}

uint8_t SpiTxRx(uint8_t value) {
    uint8_t rx = 0xFF;
    spi_write_read_blocking(kSpi, &value, &rx, 1);
    return rx;
}

uint8_t WaitReady(uint32_t timeoutMs) {
    absolute_time_t until = make_timeout_time_ms(timeoutMs);
    while (!time_reached(until)) {
        if (SpiTxRx(0xFF) == 0xFF) {
            return 1;
        }
        sleep_us(100);
    }
    return 0;
}

uint8_t SendCommand(uint8_t cmd, uint32_t arg) {
    uint8_t crc = 0x01;
    if (cmd == kCmd0) {
        crc = 0x95;
    } else if (cmd == kCmd8) {
        crc = 0x87;
    }

    DeselectCard();
    SelectCard();

    if (!WaitReady(500)) {
        DeselectCard();
        return 0xFF;
    }

    SpiTxRx(static_cast<uint8_t>(0x40 | cmd));
    SpiTxRx(static_cast<uint8_t>(arg >> 24));
    SpiTxRx(static_cast<uint8_t>(arg >> 16));
    SpiTxRx(static_cast<uint8_t>(arg >> 8));
    SpiTxRx(static_cast<uint8_t>(arg));
    SpiTxRx(crc);

    for (int n = 0; n < kCmdRetries; ++n) {
        uint8_t response = SpiTxRx(0xFF);
        if ((response & 0x80u) == 0) {
            return response;
        }
    }

    return 0xFF;
}

bool ReadSector(uint32_t sector, uint8_t* out) {
    uint32_t address = g_highCapacity ? sector : (sector << 9);
    uint8_t response = SendCommand(kCmd17, address);
    if (response != 0x00) {
        DeselectCard();
        return false;
    }

    for (int n = 0; n < kReadTokenRetries; ++n) {
        uint8_t token = SpiTxRx(0xFF);
        if (token == kDataToken) {
            spi_read_blocking(kSpi, 0xFF, out, 512);
            SpiTxRx(0xFF);
            SpiTxRx(0xFF);
            DeselectCard();
            return true;
        }
        if (token != 0xFF) {
            break;
        }
    }

    DeselectCard();
    return false;
}

bool InitSdCardSpi() {
    spi_init(kSpi, kInitBaudrateHz);
    gpio_set_function(Config::Gpio::SdMiso, GPIO_FUNC_SPI);
    gpio_set_function(Config::Gpio::SdMosi, GPIO_FUNC_SPI);
    gpio_set_function(Config::Gpio::SdSck, GPIO_FUNC_SPI);

    gpio_init(Config::Gpio::SdCs);
    gpio_set_dir(Config::Gpio::SdCs, GPIO_OUT);
    gpio_put(Config::Gpio::SdCs, 1);

    gpio_pull_up(Config::Gpio::SdMiso);

    for (int i = 0; i < 10; ++i) {
        SpiTxRx(0xFF);
    }

    uint8_t r1 = 0xFF;
    for (int i = 0; i < kInitRetries; ++i) {
        r1 = SendCommand(kCmd0, 0);
        if (r1 == 0x01) {
            break;
        }
        sleep_ms(10);
    }
    if (r1 != 0x01) {
        DeselectCard();
        return false;
    }

    g_highCapacity = false;

    r1 = SendCommand(kCmd8, 0x1AA);
    if (r1 == 0x01) {
        uint8_t r7[4];
        r7[0] = SpiTxRx(0xFF);
        r7[1] = SpiTxRx(0xFF);
        r7[2] = SpiTxRx(0xFF);
        r7[3] = SpiTxRx(0xFF);
        if (r7[2] != 0x01 || r7[3] != 0xAA) {
            DeselectCard();
            return false;
        }

        for (int i = 0; i < kInitRetries; ++i) {
            SendCommand(kCmd55, 0);
            r1 = SendCommand(kAcmd41, 0x40000000u);
            if (r1 == 0x00) {
                break;
            }
            sleep_ms(10);
        }
        if (r1 != 0x00) {
            DeselectCard();
            return false;
        }

        r1 = SendCommand(kCmd58, 0);
        if (r1 != 0x00) {
            DeselectCard();
            return false;
        }
        uint8_t ocr0 = SpiTxRx(0xFF);
        SpiTxRx(0xFF);
        SpiTxRx(0xFF);
        SpiTxRx(0xFF);
        g_highCapacity = (ocr0 & 0x40u) != 0;
    } else {
        for (int i = 0; i < kInitRetries; ++i) {
            SendCommand(kCmd55, 0);
            r1 = SendCommand(kAcmd41, 0);
            if (r1 == 0x00) {
                break;
            }
            sleep_ms(10);
        }
        if (r1 != 0x00) {
            DeselectCard();
            return false;
        }
        r1 = SendCommand(kCmd16, 512);
        if (r1 != 0x00) {
            DeselectCard();
            return false;
        }
    }

    DeselectCard();
    spi_set_baudrate(kSpi, kTransferBaudrateHz);
    return true;
}
}  // namespace

extern "C" DSTATUS disk_initialize(void) {
    g_sdReady = InitSdCardSpi();
    return g_sdReady ? 0 : STA_NOINIT;
}

extern "C" DRESULT disk_readp(BYTE* buff, DWORD sector, UINT offset, UINT count) {
    static uint8_t sectorBuffer[512];

    if (!g_sdReady) {
        return RES_NOTRDY;
    }
    if (offset > sizeof(sectorBuffer) || count > sizeof(sectorBuffer) - offset) {
        return RES_PARERR;
    }

    if (!ReadSector(sector, sectorBuffer)) {
        return RES_ERROR;
    }

    if (buff != nullptr && count != 0) {
        std::memcpy(buff, sectorBuffer + offset, count);
    }

    return RES_OK;
}

extern "C" DRESULT disk_writep(const BYTE* buff, DWORD sc) {
    (void)buff;
    (void)sc;
    return RES_PARERR;
}

bool InitSdCardPetitFatFs() {
    if (disk_initialize() & STA_NOINIT) {
        return false;
    }

    return pf_mount(&g_fs) == FR_OK;
}

// bool ReadSdCardFile(const char* path, uint8_t* buffer, size_t bufferSize, size_t* readBytes) {
//     if (readBytes != nullptr) {
//         *readBytes = 0;
//     }
//     if (path == nullptr || buffer == nullptr) {
//         return false;
//     }
//     if (!g_sdReady && !InitSdCardPetitFatFs()) {
//         return false;
//     }

//     if (pf_open(path) != FR_OK) {
//         return false;
//     }

//     UINT br = 0;
//     if (pf_read(buffer, static_cast<UINT>(bufferSize), &br) != FR_OK) {
//         return false;
//     }

//     if (readBytes != nullptr) {
//         *readBytes = br;
//     }
//     return true;
// }
