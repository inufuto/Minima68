#include "Color.h"

void Color::SetRgb(uint8_t r, uint8_t g, uint8_t b)
{
    int32_t y = (150 * g + 29 * b + 77 * r + 128) / 256;
    int32_t b_y_1 = (b - y) * 441;
	int32_t r_y_1 = (r - y) * 1361;
	int32_t b_y_2 = (b - y) * 764;
	int32_t r_y_2 = (r - y) * (-786);

    values[0] = To16(y, b_y_1, r_y_1);
    values[1] = To16(y, b_y_2, r_y_2);
    values[2] = To16(y, -b_y_1, -r_y_1);
    values[3] = To16(y, -b_y_2, -r_y_2);
}
