#include <stdint.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include <hid.h>

extern "C" {
#include "diskio.h"
#include "pff.h"
}

#include "Config.h"
#include "Input.h"
#include "Minima68Pico.h"
#include "SdCardPetitFatFs.h"

struct {
    uint8_t code;
    uint8_t bit;
} const KeyMap[] = {
    { HID_KEY_ARROW_LEFT, Joystick::Left },
    { HID_KEY_ARROW_RIGHT, Joystick::Right },
    { HID_KEY_ARROW_UP, Joystick::Up },
    { HID_KEY_ARROW_DOWN, Joystick::Down },
    { HID_KEY_Z, Joystick::Button0 },
    { HID_KEY_X, Joystick::Button1 },
    { HID_KEY_SPACE, Joystick::Button0 },
    { HID_KEY_ENTER, Joystick::Button1 },
};

struct KeyMapElement {
    uint8_t code;
    char character;
    char shiftedCharacter;
};
const KeyMapElement Key101ToChar[] = {
    { HID_KEY_A, 'a', 'A' },
    { HID_KEY_B, 'b', 'B' },
    { HID_KEY_C, 'c', 'C' },
    { HID_KEY_D, 'd', 'D' },
    { HID_KEY_E, 'e', 'E' },
    { HID_KEY_F, 'f', 'F' },
    { HID_KEY_G, 'g', 'G' },
    { HID_KEY_H, 'h', 'H' },
    { HID_KEY_I, 'i', 'I' },
    { HID_KEY_J, 'j', 'J' },
    { HID_KEY_K, 'k', 'K' },
    { HID_KEY_L, 'l', 'L' },
    { HID_KEY_M, 'm', 'M' },
    { HID_KEY_N, 'n', 'N' },
    { HID_KEY_O, 'o', 'O' },
    { HID_KEY_P, 'p', 'P' },
    { HID_KEY_Q, 'q', 'Q' },
    { HID_KEY_R, 'r', 'R' },
    { HID_KEY_S, 's', 'S' },
    { HID_KEY_T, 't', 'T' },
    { HID_KEY_U, 'u', 'U' },
    { HID_KEY_V, 'v', 'V' },
    { HID_KEY_W, 'w', 'W' },
    { HID_KEY_X, 'x', 'X' },
    { HID_KEY_Y, 'y', 'Y' },
    { HID_KEY_Z, 'z', 'Z' },
    { HID_KEY_1, '1', '!' },
    { HID_KEY_2, '2', '@' },
    { HID_KEY_3, '3', '#' },
    { HID_KEY_4, '4', '$' },
    { HID_KEY_5, '5', '%' },
    { HID_KEY_6, '6', '^' },
    { HID_KEY_7, '7', '&' },
    { HID_KEY_8, '8', '*' },
    { HID_KEY_9, '9', '(' },
    { HID_KEY_0, '0', ')' },
    { HID_KEY_SPACE, ' ', ' ' },
    { HID_KEY_MINUS, '-', '_' },
    { HID_KEY_EQUAL, '=', '+' },
    { HID_KEY_BRACKET_LEFT, '[', '{' },
    { HID_KEY_BRACKET_RIGHT, ']', '}' },
    { HID_KEY_BACKSLASH, '\\', '|' },
    { HID_KEY_SEMICOLON, ';', ':' },
    { HID_KEY_APOSTROPHE, '\'', '"' },
    { HID_KEY_GRAVE, '`', '~' },
    { HID_KEY_COMMA, ',', '<' },
    { HID_KEY_PERIOD, '.', '>' },
    { HID_KEY_SLASH, '/', '?' },
    { HID_KEY_KEYPAD_0, '0', '0' },
    { HID_KEY_KEYPAD_1, '1', '1' },
    { HID_KEY_KEYPAD_2, '2', '2' },
    { HID_KEY_KEYPAD_3, '3', '3' },
    { HID_KEY_KEYPAD_4, '4', '4' },
    { HID_KEY_KEYPAD_5, '5', '5' },
    { HID_KEY_KEYPAD_6, '6', '6' },
    { HID_KEY_KEYPAD_7, '7', '7' },
    { HID_KEY_KEYPAD_8, '8', '8' },
    { HID_KEY_KEYPAD_9, '9', '9' },
    { HID_KEY_KEYPAD_DECIMAL, '.', '.' },
    { HID_KEY_KEYPAD_ADD, '+', '+' },
    { HID_KEY_KEYPAD_SUBTRACT, '-', '-' },
    { HID_KEY_KEYPAD_MULTIPLY, '*', '*' },
    { HID_KEY_KEYPAD_DIVIDE, '/', '/' },
    {0, 0, 0},
};
const KeyMapElement Key106ToChar[] = {
    { HID_KEY_A, 'a', 'A' },
    { HID_KEY_B, 'b', 'B' },
    { HID_KEY_C, 'c', 'C' },
    { HID_KEY_D, 'd', 'D' },
    { HID_KEY_E, 'e', 'E' },
    { HID_KEY_F, 'f', 'F' },
    { HID_KEY_G, 'g', 'G' },
    { HID_KEY_H, 'h', 'H' },
    { HID_KEY_I, 'i', 'I' },
    { HID_KEY_J, 'j', 'J' },
    { HID_KEY_K, 'k', 'K' },
    { HID_KEY_L, 'l', 'L' },
    { HID_KEY_M, 'm', 'M' },
    { HID_KEY_N, 'n', 'N' },
    { HID_KEY_O, 'o', 'O' },
    { HID_KEY_P, 'p', 'P' },
    { HID_KEY_Q, 'q', 'Q' },
    { HID_KEY_R, 'r', 'R' },
    { HID_KEY_S, 's', 'S' },
    { HID_KEY_T, 't', 'T' },
    { HID_KEY_U, 'u', 'U' },
    { HID_KEY_V, 'v', 'V' },
    { HID_KEY_W, 'w', 'W' },
    { HID_KEY_X, 'x', 'X' },
    { HID_KEY_Y, 'y', 'Y' },
    { HID_KEY_Z, 'z', 'Z' },
    { HID_KEY_1, '1', '!' },
    { HID_KEY_2, '2', '"' },
    { HID_KEY_3, '3', '#' },
    { HID_KEY_4, '4', '$' },
    { HID_KEY_5, '5', '%' },
    { HID_KEY_6, '6', '&' },
    { HID_KEY_7, '7', '\'' },
    { HID_KEY_8, '8', '(' },
    { HID_KEY_9, '9', ')' },
    { HID_KEY_0, '0', ')' },
    { HID_KEY_SPACE, ' ', ' ' },
    { HID_KEY_MINUS, '-', '=' },
    { HID_KEY_EQUAL, '^', '~' },
    { HID_KEY_BRACKET_LEFT, '@', '`' },
    { HID_KEY_BRACKET_RIGHT, '[', '{' },
    { HID_KEY_BACKSLASH, ']', '}' },
    { HID_KEY_SEMICOLON, ';', '+' },
    { HID_KEY_APOSTROPHE, ':', '*' },
    { HID_KEY_GRAVE, '`', '~' },
    { HID_KEY_COMMA, ',', '<' },
    { HID_KEY_PERIOD, '.', '>' },
    { HID_KEY_SLASH, '/', '?' },
    { 0x89, '\\', '|' },
    { 0x87, 0, '_' },
    { HID_KEY_KEYPAD_0, '0', '0' },
    { HID_KEY_KEYPAD_1, '1', '1' },
    { HID_KEY_KEYPAD_2, '2', '2' },
    { HID_KEY_KEYPAD_3, '3', '3' },
    { HID_KEY_KEYPAD_4, '4', '4' },
    { HID_KEY_KEYPAD_5, '5', '5' },
    { HID_KEY_KEYPAD_6, '6', '6' },
    { HID_KEY_KEYPAD_7, '7', '7' },
    { HID_KEY_KEYPAD_8, '8', '8' },
    { HID_KEY_KEYPAD_9, '9', '9' },
    { HID_KEY_KEYPAD_DECIMAL, '.', '.' },
    { HID_KEY_KEYPAD_ADD, '+', '+' },
    { HID_KEY_KEYPAD_SUBTRACT, '-', '-' },
    { HID_KEY_KEYPAD_MULTIPLY, '*', '*' },
    { HID_KEY_KEYPAD_DIVIDE, '/', '/' },
    {0, 0, 0},
};

const KeyMapElement* pKeyMapToChar = Key106ToChar;

extern "C" {

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
    tuh_hid_set_protocol(dev_addr, instance, HID_PROTOCOL_BOOT);
    tuh_hid_receive_report(dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, const uint8_t *report, uint16_t len)
{   
    auto kb_report = reinterpret_cast<hid_keyboard_report_t const*>(report);
    auto bits = 0;
    bool controlKey = kb_report->modifier & (KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_RIGHTCTRL) != 0;
    for (auto i = 0; i < TU_ARRAY_SIZE(kb_report->keycode); ++i) {
        for (const auto& element: KeyMap) {
            if (element.code == kb_report->keycode[i]) {
                bits |= element.bit;
                break;
            }
        }
        for (auto p = pKeyMapToChar; p->code != 0; ++p) {
            if (p->code == kb_report->keycode[i]) {
                char charCode = controlKey ? 0 : (kb_report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT) ? p->shiftedCharacter : p->character);
                if (charCode != 0) {
                    emulator.AddKeyCode(charCode);
                }
                break;
            }
        }
    }
    tuh_hid_receive_report(dev_addr, instance);
    emulator.SetJoystickBits(bits);
}

} // extern "C"

static void InitButton(uint gpio)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
}

void InitInput()
{
    InitButton(Config::Gpio::UpButton);
    InitButton(Config::Gpio::LeftButton);
    InitButton(Config::Gpio::RightButton);
    InitButton(Config::Gpio::DownButton);
    InitButton(Config::Gpio::Button0);
    InitButton(Config::Gpio::Button1);

    if (pf_open("MINIMA68.INI") == 0) {
        char buffer[256];
        while (ReadTextLine(buffer, sizeof(buffer)) > 0) {
            if (strncmp(buffer, "101KEY", 6) == 0) {
                pKeyMapToChar = Key101ToChar;
            }
            else if (strncmp(buffer, "106KEY", 6) == 0) {
                pKeyMapToChar = Key106ToChar;
            }
        }
    }
}


uint8_t ReadButtons()
{
    auto all = gpio_get_all();
    uint8_t bits = 0;
    if ((all & (1u << Config::Gpio::UpButton)) == 0) {
        bits |= Joystick::Up;
    }
    if ((all & (1u << Config::Gpio::LeftButton)) == 0) {
        bits |= Joystick::Left;
    }
    if ((all & (1u << Config::Gpio::RightButton)) == 0) {
        bits |= Joystick::Right;
    }
    if ((all & (1u << Config::Gpio::DownButton)) == 0) {
        bits |= Joystick::Down;
    }
    if ((all & (1u << Config::Gpio::Button0)) == 0) {
        bits |= Joystick::Button0;
    }
    if ((all & (1u << Config::Gpio::Button1)) == 0) {
        bits |= Joystick::Button1;
    }
    return bits;
}