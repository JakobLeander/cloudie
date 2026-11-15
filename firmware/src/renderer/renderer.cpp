#include "renderer.h"
#include "display/display.h"
#include <Adafruit_GFX.h>

namespace
{
    GFXcanvas16 canvas(display::WIDTH, display::HEIGHT);

    void drawFaceBase(uint16_t faceColor)
    {
        canvas.fillScreen(0x0000);
        canvas.fillCircle(64, 64, 48, faceColor);
        canvas.drawCircle(64, 64, 48, 0x0000);
    }
}

void renderer::drawHappyFace()
{
    constexpr uint16_t black = 0x0000;
    constexpr uint16_t yellow = 0xFFE0;

    drawFaceBase(yellow);

    canvas.fillCircle(49, 49, 4, black);
    canvas.fillCircle(79, 49, 4, black);

    canvas.drawLine(43, 73, 50, 82, black);
    canvas.drawLine(50, 82, 58, 88, black);
    canvas.drawLine(58, 88, 64, 90, black);
    canvas.drawLine(64, 90, 70, 88, black);
    canvas.drawLine(70, 88, 78, 82, black);
    canvas.drawLine(78, 82, 85, 73, black);

    present();
}

void renderer::drawSadFace()
{
    constexpr uint16_t black = 0x0000;
    constexpr uint16_t cyan = 0x07FF;

    drawFaceBase(cyan);

    canvas.fillCircle(49, 49, 4, black);
    canvas.fillCircle(79, 49, 4, black);

    canvas.drawLine(43, 88, 50, 80, black);
    canvas.drawLine(50, 80, 58, 75, black);
    canvas.drawLine(58, 75, 64, 73, black);
    canvas.drawLine(64, 73, 70, 75, black);
    canvas.drawLine(70, 75, 78, 80, black);
    canvas.drawLine(78, 80, 85, 88, black);

    present();
}

void renderer::drawNeutralFace()
{
    constexpr uint16_t black = 0x0000;
    constexpr uint16_t green = 0x07E0;

    drawFaceBase(green);
    canvas.fillCircle(49, 49, 4, black);
    canvas.fillCircle(79, 49, 4, black);
    canvas.drawLine(51, 80, 77, 80, black);

    present();
}

void renderer::drawCuriousFace()
{
    constexpr uint16_t black = 0x0000;
    constexpr uint16_t orange = 0xFD20;

    drawFaceBase(orange);
    canvas.fillCircle(48, 49, 5, black);
    canvas.fillCircle(79, 49, 4, black);
    canvas.drawLine(72, 39, 84, 36, black);
    canvas.drawCircle(64, 80, 4, black);

    present();
}

void renderer::drawThinkingFace()
{
    constexpr uint16_t black = 0x0000;
    constexpr uint16_t white = 0xFFFF;
    constexpr uint16_t magenta = 0xF81F;

    drawFaceBase(magenta);
    canvas.fillCircle(49, 49, 6, white);
    canvas.fillCircle(79, 49, 6, white);
    canvas.fillCircle(52, 46, 2, black);
    canvas.fillCircle(82, 46, 2, black);
    canvas.drawLine(43, 40, 54, 42, black);
    canvas.drawLine(75, 40, 85, 37, black);

    canvas.fillCircle(91, 35, 2, black);
    canvas.fillCircle(97, 29, 3, black);
    canvas.fillCircle(104, 21, 5, black);
    canvas.fillCircle(104, 21, 3, magenta);
    canvas.drawLine(60, 82, 68, 80, black);

    present();
}

void renderer::present()
{
    display::drawBitmap565(canvas.getBuffer());
}