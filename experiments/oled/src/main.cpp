#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#define CS_PIN 5
#define DC_PIN 16
#define RST_PIN 17

// Hardware SPI: MOSI=23, SCK=18 by default
Adafruit_SSD1351 tft(128, 128, &SPI, CS_PIN, DC_PIN, RST_PIN);
GFXcanvas16 frame(128, 128);

const uint16_t colors[] = {
    0x07FF, // Cyan
    0xF81F, // Magenta
    0xFFE0, // Yellow
    0x07E0, // Green
    0xFD20, // Orange
    0x001F  // Blue
};

float phase = 0.0f;

void drawThickLine(int x0, int y0, int x1, int y1, uint16_t color,
                   bool vertical)
{
  for (int offset = -1; offset <= 1; offset++)
  {
    if (vertical)
    {
      frame.drawLine(x0 + offset, y0, x1 + offset, y1, color);
    }
    else
    {
      frame.drawLine(x0, y0 + offset, x1, y1 + offset, color);
    }
  }
}

void drawPattern(float patternPhase)
{
  for (int band = 0; band < 3; band++)
  {
    const float offset = patternPhase + band * 0.72f;
    const float amplitude = 24.0f + band * 8.0f;
    const uint16_t color = colors[band];

    int previousY = 64 + static_cast<int>(
                             sin(offset) * amplitude +
                             sin(-offset * 1.4f) * 10.0f);
    for (int x = 6; x <= 128; x += 6)
    {
      const float wave = sin(x * 0.07f + offset) * amplitude +
                         sin(x * 0.025f - offset * 1.4f) * 10.0f;
      const int y = 64 + static_cast<int>(wave);
      drawThickLine(x - 6, previousY, x, y, color, false);
      previousY = y;
    }

    int previousX = 64 + static_cast<int>(
                             sin(-offset) * amplitude +
                             sin(offset * 1.4f) * 10.0f);
    const uint16_t crossingColor = colors[(band + 3) % 6];
    for (int y = 6; y <= 128; y += 6)
    {
      const float wave = sin(y * 0.07f - offset) * amplitude +
                         sin(y * 0.025f + offset * 1.4f) * 10.0f;
      const int x = 64 + static_cast<int>(wave);
      drawThickLine(previousX, y - 6, x, y, crossingColor, true);
      previousX = x;
    }
  }

}

void setup()
{
  SPI.begin(18, -1, 23, CS_PIN); // SCK, MISO (unused), MOSI, SS
  tft.begin(8000000);

  uint8_t channelContrast[] = {0xFF, 0xFF, 0xFF};
  uint8_t masterContrast = 0x0F;
  tft.sendCommand(SSD1351_CMD_CONTRASTABC, channelContrast, 3);
  tft.sendCommand(SSD1351_CMD_CONTRASTMASTER, &masterContrast, 1);

  tft.fillScreen(0x0000);
}

void loop()
{
  frame.fillScreen(0x0000);

  phase += 0.10f;
  if (phase >= 6.28318f)
  {
    phase -= 6.28318f;
  }

  drawPattern(phase);
  tft.drawRGBBitmap(0, 0, frame.getBuffer(), 128, 128);
  delay(25);
}