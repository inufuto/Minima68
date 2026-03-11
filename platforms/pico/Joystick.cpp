#include <stdint.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include <hid.h>

#include "Config.h"
#include "Joystick.h"
#include "Minima68Pico.h"

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

void InitButtons()
{
    InitButton(Config::Gpio::UpButton);
    InitButton(Config::Gpio::LeftButton);
    InitButton(Config::Gpio::RightButton);
    InitButton(Config::Gpio::DownButton);
    InitButton(Config::Gpio::Button0);
    InitButton(Config::Gpio::Button1);
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
