
#include <Arduino.h>
#include "display/display.h"
#include "network/WiFiManager.h"
#include "network/StateClient.h"
#include "renderer/renderer.h"

namespace
{
    Renderer renderer(display::WIDTH, display::HEIGHT);
    WiFiManager wifiManager;
    Renderer::Animation animation = Renderer::Animation::Waiting;
    StateClient stateClient;

    uint32_t lastAnimationChange = 0;
    std::string lastLoggedState;
}

void setup()
{
    Serial.begin(115200);
    wifiManager.connect();
    display::begin();
    stateClient.begin();
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

    // Read the latest state cached by StateClient.
    const std::string state = stateClient.getState();
    if (state != lastLoggedState)
    {
        Serial.println(state.c_str());
        lastLoggedState = state;
    }
}