
#include <Arduino.h>
#include "display/display.h"
#include "renderer/renderer.h"

namespace
{
    Renderer renderer(display::WIDTH, display::HEIGHT);
    Renderer::Animation animation = Renderer::Animation::Attentive;
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
        if (animation == Renderer::Animation::Sleep)
        {
            animation = Renderer::Animation::Attentive;
        }
        else
        {
            animation = Renderer::Animation::Sleep;
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