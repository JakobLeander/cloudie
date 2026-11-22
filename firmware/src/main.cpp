
#include <Arduino.h>
#include "display/display.h"
#include "renderer/renderer.h"

namespace
{
    Renderer renderer(display::WIDTH, display::HEIGHT);
    Renderer::Animation animation = Renderer::Animation::Waiting;
    uint32_t lastAnimationChange = 0;
}

void setup()
{
    display::begin();
    lastAnimationChange = millis();
}

void loop()
{
    const uint32_t now = millis();
    if (now - lastAnimationChange >= 10UL * 1000)
    {
        if (animation == Renderer::Animation::Attention)
        {
            animation = Renderer::Animation::Waiting;
        }
        else if (animation == Renderer::Animation::Waiting)
        {
            animation = Renderer::Animation::Attention;
        }

        renderer.setAnimation(animation);
        lastAnimationChange = now;
    }

    const uint16_t *bitmap = renderer.render();
    if (bitmap != nullptr)
    {
        display::drawBitmap565(bitmap);
    }
}