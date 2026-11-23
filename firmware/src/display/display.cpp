#include "Display.h"
#include "../../include/Config.h"
#include <Adafruit_SSD1351.h>
#include <SPI.h>

Adafruit_SSD1351 oled(display::WIDTH, display::HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

bool display::begin()
{
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
    oled.begin(8000000);

    // Set full channel contrast and a reduced overall brightness.
    uint8_t channelContrast[] = {0xFF, 0xFF, 0xFF};
    uint8_t masterContrast = 0x0F;
    oled.sendCommand(SSD1351_CMD_CONTRASTABC, channelContrast, 3);
    oled.sendCommand(SSD1351_CMD_CONTRASTMASTER, &masterContrast, 1);

    oled.fillScreen(0x0000);
    return true;
}

void display::drawBitmap565(const uint16_t *data)
{
    uint16_t rowBuffer[display::WIDTH];

    // Send the frame in one transaction, reversing rows for the panel orientation.
    oled.startWrite();
    for (int16_t row = 0; row < display::HEIGHT; ++row)
    {
        const uint32_t sourceRow =
            static_cast<uint32_t>(display::HEIGHT - 1 - row) * display::WIDTH;
        for (int16_t column = 0; column < display::WIDTH; ++column)
        {
            rowBuffer[column] = data[sourceRow + column];
        }

        oled.setAddrWindow(0, row, display::WIDTH, 1);
        oled.writePixels(rowBuffer, display::WIDTH);
    }
    oled.endWrite();
}
