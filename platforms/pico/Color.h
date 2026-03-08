#pragma once

#include <stdint.h>

class Color {
private:
    uint16_t values[4];
    
    uint16_t To16(int32_t y, int32_t b_y, int32_t r_y) {
        auto s = (y * 1792 + b_y + r_y + 2 * 65536 + 32768) / 65536;
        return s < 0 ? 0 : s;
    }
public:
    void SetRgb(uint8_t r, uint8_t g, uint8_t b);
    const auto& Values() const { return values; }
};
