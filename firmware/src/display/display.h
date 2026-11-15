#pragma once
#include <stdint.h>

namespace display
{
    bool begin();
    void clear(uint16_t color = 0x0000);
    constexpr int WIDTH = 128;
    constexpr int HEIGHT = 128;

    // Copy a full-screen RGB565 bitmap to the panel
    void drawBitmap565(const uint16_t *data);
}