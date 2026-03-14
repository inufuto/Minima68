#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
#include <memory>

#include <tusb.h>
#include <hid.h>

extern "C" {
#include "diskio.h"
#include "pff.h"
}

#include "Minima68Pico.h"
#include "MemoryMap.h"
#include "Video.h"
#include "SoundChannel.h"
#include "Joystick.h"
#include "version.h"

static uint8_t AsciiPattern[] = {
    //	ascii
	0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x04, 0x40,
	0xaa, 0x00, 0x00, 0x00, 0x44, 0xe4, 0xe4, 0x40,
	0x4e, 0xce, 0x6e, 0x40, 0x8a, 0x24, 0x8a, 0x20,
	0x4a, 0x48, 0xaa, 0x40, 0x44, 0x40, 0x00, 0x00,
	0x48, 0x88, 0x88, 0x40, 0x42, 0x22, 0x22, 0x40,
	0x0a, 0xa4, 0xaa, 0x00, 0x04, 0x4e, 0x44, 0x00,
	0x00, 0x00, 0x44, 0x80, 0x00, 0x0e, 0x00, 0x00,
	0x00, 0x00, 0x04, 0x40, 0x02, 0x24, 0x88, 0x00,
	0x4a, 0xaa, 0xaa, 0x40, 0x22, 0x22, 0x22, 0x20,
	0x4a, 0x22, 0x48, 0xe0, 0x4a, 0x24, 0x2a, 0x40,
	0x2a, 0xae, 0x22, 0x20, 0xe8, 0x8c, 0x22, 0xc0,
	0x4a, 0x8c, 0xaa, 0x40, 0xea, 0x24, 0x44, 0x40,
	0x4a, 0xa4, 0xaa, 0x40, 0x4a, 0xa6, 0x2a, 0x40,
	0x04, 0x40, 0x44, 0x00, 0x04, 0x40, 0x44, 0x80,
	0x02, 0x48, 0x42, 0x00, 0x00, 0xe0, 0xe0, 0x00,
	0x08, 0x42, 0x48, 0x00, 0x4a, 0x24, 0x40, 0x40,
	0x4a, 0xaa, 0x88, 0x60, 0x4a, 0xaa, 0xea, 0xa0,
	0xca, 0xac, 0xaa, 0xc0, 0x4a, 0x88, 0x8a, 0x40,
	0xca, 0xaa, 0xaa, 0xc0, 0xe8, 0x8c, 0x88, 0xe0,
	0xe8, 0x8c, 0x88, 0x80, 0x68, 0x8a, 0xaa, 0x40,
	0xaa, 0xae, 0xaa, 0xa0, 0xe4, 0x44, 0x44, 0xe0,
	0x62, 0x22, 0x2a, 0x40, 0xae, 0xc8, 0xce, 0xa0,
	0x88, 0x88, 0x88, 0xe0, 0xae, 0xee, 0xaa, 0xa0,
	0xca, 0xaa, 0xaa, 0xa0, 0x4a, 0xaa, 0xaa, 0x40,
	0xca, 0xaa, 0xc8, 0x80, 0x4a, 0xaa, 0xac, 0x60,
	0xca, 0xaa, 0xca, 0xa0, 0x4a, 0x84, 0x2a, 0x40,
	0xe4, 0x44, 0x44, 0x40, 0xaa, 0xaa, 0xaa, 0xe0,
	0xaa, 0xaa, 0xa4, 0x40, 0xaa, 0xae, 0xee, 0xa0,
	0xaa, 0x44, 0x4a, 0xa0, 0xaa, 0xa4, 0x44, 0x40,
	0xe2, 0x24, 0x88, 0xe0, 0x64, 0x44, 0x44, 0x60,
	0x08, 0x84, 0x22, 0x00, 0xc4, 0x44, 0x44, 0xc0,
	0x4a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
	0x44, 0x20, 0x00, 0x00, 0x00, 0xc2, 0xea, 0xe0,
	0x88, 0xea, 0xaa, 0xe0, 0x00, 0xe8, 0x88, 0xe0,
	0x22, 0xea, 0xaa, 0xe0, 0x00, 0xea, 0xe8, 0xe0,
	0x24, 0x4e, 0x44, 0x40, 0x00, 0xea, 0xae, 0x2e,
	0x88, 0xea, 0xaa, 0xa0, 0x40, 0x44, 0x44, 0x40,
	0x20, 0x22, 0x22, 0xa4, 0x88, 0xac, 0x8c, 0xa0,
	0x44, 0x44, 0x44, 0x60, 0x00, 0xae, 0xea, 0xa0,
	0x00, 0xca, 0xaa, 0xa0, 0x00, 0xea, 0xaa, 0xe0,
	0x00, 0xea, 0xaa, 0xe8, 0x00, 0xea, 0xaa, 0xe2,
	0x00, 0xca, 0x88, 0x80, 0x00, 0xe8, 0xe2, 0xe0,
	0x44, 0xe4, 0x44, 0x60, 0x00, 0xaa, 0xaa, 0xe0,
	0x00, 0xaa, 0xaa, 0x40, 0x00, 0xaa, 0xee, 0xa0,
	0x00, 0xaa, 0x4a, 0xa0, 0x00, 0xaa, 0xae, 0x2e,
	0x00, 0xe2, 0x48, 0xe0, 0x24, 0x48, 0x44, 0x20,
	0x44, 0x44, 0x44, 0x40, 0x84, 0x42, 0x44, 0x80,
	0xa4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const uint8_t SquareWave[] = {
	254,254,254,254,254,254,254,254,
	254,254,254,254,254,254,254,254,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};

constexpr auto AsciiColor = 7;
constexpr auto VisibleLineCount = VramHeight - 3;
constexpr auto LeftX = 8;

struct FileEntry {
    std::string name;
    bool directory;
};

static std::string CurrentDirectory = "";
static const char* ParentDirectory = "..";
static const char* Suffix = ".BIN";
static std::vector<std::unique_ptr<FileEntry>> FileEntries;
static int TopIndex;
static int CurrentIndex;

static void InitAsciiPattern() 
{
    auto pSource = AsciiPattern;
    auto pDestination = emulator.Ram() + TilePatternAddress + TilePatternSize * 0x20;
    for (auto i = 0; i < 4 * 0x60; ++i) {
        auto source = *pSource++;
        for (auto j = 0; j < 4; ++j) {
            auto destination = 0;
            for (auto k = 0; k < 2; ++k) {
                destination <<= 4;
                if ((source & 0x80) != 0) {
                    destination |= AsciiColor;
                }
                source <<= 1;
            }
            *pDestination++ = destination;
        }
    }
}

static void InitColors() 
{
    for (auto i = 0; i < ColorCount; ++i) {
        uint8_t b = (i & 1) != 0 ? 0xff : 0x00;
        uint8_t r = (i & 2) != 0 ? 0xff : 0x00;
        uint8_t g = (i & 4) != 0 ? 0xff : 0x00;
        emulator.SetColor(i, r, g, b);
    }
}

static void ReadDirectory()
{
    TopIndex = CurrentIndex = 0;
    FileEntries.clear();
    DIR dir;
    if ((pf_opendir(&dir, CurrentDirectory.c_str()) == FR_OK)) {
        if (!CurrentDirectory.empty()) {
            auto pFileEntry = std::make_unique<FileEntry>();
            pFileEntry->name = ParentDirectory;
            pFileEntry->directory = true;
            FileEntries.push_back(std::move(pFileEntry));
        }
        while (true) {
            auto pFileEntry = std::make_unique<FileEntry>();
            FILINFO fileInfo;
            if (pf_readdir(&dir, &fileInfo) != FR_OK || fileInfo.fname[0] == '\0') {
                break;
            }
            pFileEntry->name = fileInfo.fname;
            pFileEntry->directory = (fileInfo.fattrib & AM_DIR) != 0;
            if (!pFileEntry->directory) {
                auto length = pFileEntry->name.length();
                if (length < strlen(Suffix) || pFileEntry->name.compare(length - strlen(Suffix), strlen(Suffix), Suffix) != 0) {
                    continue;
                }
            }
            FileEntries.push_back(std::move(pFileEntry));
        }
    }
}

static uint16_t PrintC(uint16_t vram, char c)
{
    emulator.Ram()[TileMapAddress + vram++] = c;
    return vram;
}

static uint16_t PrintS(uint16_t vram, const char* s)
{
    char c;
    while ((c = *s++) != 0) {
        vram = PrintC(vram, c);
    }
    return vram;
}

static void ShowList() 
{
    memset(emulator.Ram() + TileMapAddress, ' ', VramWidth * VramHeight);
    std::string title = "Minima68 " APP_VERSION;
    PrintS((WindowWidth - title.length()) / 2, title.c_str());
    auto vram = PrintS(VramWidth * 2 + LeftX, CurrentDirectory.c_str());
    PrintC(vram, '/');
    auto line = VramWidth * 3;
    for (auto i = 0; i < VisibleLineCount; ++i) {
        auto index = TopIndex + i;
        if (index >= FileEntries.size()) break;
        const auto& pFileEntry = FileEntries[index];
        vram = line + LeftX;
        vram = PrintC(vram, index == CurrentIndex ? '>' : ' ');
        vram = PrintC(vram, ' ');
        vram = PrintS(vram, pFileEntry->name.c_str());
        if (pFileEntry->directory) {
            PrintC(vram, '/');
        }
        line += VramWidth;
    }
}

inline uint8_t ReadJoystick()
{
    tuh_task();
    return emulator.ReadMemory(JoystickAddress) & Joystick::All;
}

static void Loop()
{
    auto choosed = false;
    ReadDirectory();
    ShowList();
    do {
        uint8_t button;
        do {
            button = ReadJoystick();
        } while (button == 0);
        if ((button & Joystick::Button0) != 0) {
            if (FileEntries[CurrentIndex]->directory) {
                toneChannels[0].SetVolume(MaxVolume);
                if (FileEntries[CurrentIndex]->name == ParentDirectory) {
                    auto pos = CurrentDirectory.find_last_of('/', CurrentDirectory.length() - 2);
                    if (pos == std::string::npos || pos == 0) {
                        CurrentDirectory = "";
                    }
                    else {
                        CurrentDirectory = CurrentDirectory.substr(0, pos);
                    }
                }
                else {
                    if (!CurrentDirectory.empty()) {
                        CurrentDirectory += "/";
                    }
                    CurrentDirectory += FileEntries[CurrentIndex]->name;
                }
                while (ReadJoystick() != 0);
                toneChannels[0].SetVolume(0);
                ReadDirectory();
                ShowList();
                continue;
            }
            choosed = true;
            break;
        }
        auto direction = 0;
        if ((button & Joystick::Up) != 0) {
            --direction;
        }
        if ((button & Joystick::Down) != 0) {
            ++direction;
        }
        if (direction != 0) {
            toneChannels[0].SetVolume(MaxVolume);
            auto newIndex = CurrentIndex + direction;
            if (newIndex >= 0 && newIndex < FileEntries.size()) {
                CurrentIndex = newIndex;
                if (CurrentIndex < TopIndex) {
                    TopIndex = CurrentIndex;
                }
                else if (CurrentIndex >= TopIndex + VisibleLineCount) {
                    TopIndex = CurrentIndex - VisibleLineCount + 1;
                }
            }
            while (ReadJoystick() != 0);
            toneChannels[0].SetVolume(0);
            ShowList();
        }
    } while (!choosed);
}

void RunLauncher()
{
    InitAsciiPattern();
    InitColors();
    toneChannels[0].SetSamples(SquareWave);
    toneChannels[0].SetFrequency(440);
    toneChannels[0].SetVolume(0);
    Loop();

    memset(emulator.Ram() + TileMapAddress, 0, VramWidth * VramHeight);
    auto path = CurrentDirectory + "/" + FileEntries[CurrentIndex]->name;
    pf_open(path.c_str());
    pf_read(
        emulator.Ram() + Minima68::StartAddress,
        static_cast<UINT>(TilePatternAddress - Minima68::StartAddress), nullptr
    );
}