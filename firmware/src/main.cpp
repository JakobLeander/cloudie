
#include <Arduino.h>
#include "display/display.h"
#include "network/WiFiManager.h"
#include "network/StateClient.h"
#include "renderer/renderer.h"

namespace
{
    static const uint32_t ATTENTION_AFTER_MS = 10UL * 1000; // Wait this long before shifting to attention animation
    static const uint32_t SLEEP_AFTER_MS = 60UL * 1000;     // Wait this long before shifting to sleep animation

    Renderer renderer(display::WIDTH, display::HEIGHT);
    WiFiManager wifiManager;
    Renderer::Animation animation = Renderer::Animation::Waiting;
    StateClient stateClient;

    uint32_t lastRecievedStateChange = 0;
    std::string lastRecievedState;
}

void setup()
{
    Serial.begin(115200);
    wifiManager.connect();
    display::begin();
    stateClient.begin();
    lastRecievedStateChange = millis();
}

void loop()
{
    const uint32_t now = millis();

    // Read the latest state cached by StateClient.
    const std::string state = stateClient.getState();
    if (state != lastRecievedState)
    {
        Serial.println(state.c_str());
        lastRecievedState = state;
        lastRecievedStateChange = now;

        // Set animation to match the state.
        if (state == "Sleep")
        {
            animation = Renderer::Animation::Sleep;
        }
        else if (state == "Waiting")
        {
            animation = Renderer::Animation::Waiting;
        }
        else if (state == "Happy")
        {
            animation = Renderer::Animation::Happy;
        }
        else if (state == "Sad")
        {
            animation = Renderer::Animation::Sad;
        }
        else if (state == "Think")
        {
            animation = Renderer::Animation::Think;
        }
        else if (state == "Attention")
        {
            animation = Renderer::Animation::Attention;
        }
        renderer.setAnimation(animation);
    }

    // Set to idle animation if no activity for a while
    if (now - lastRecievedStateChange >= ATTENTION_AFTER_MS)
    {
        // set to sleep if no activity for longer while
        if (now - lastRecievedStateChange >= SLEEP_AFTER_MS)
        {
            animation = Renderer::Animation::Sleep;
        }
        else
        {
            animation = Renderer::Animation::Waiting;
        }

        renderer.setAnimation(animation);
    }

    // Render the current animation frame and draw it to the display.
    const uint16_t *bitmap = renderer.render();
    if (bitmap != nullptr)
    {
        display::drawBitmap565(bitmap);
    }
}