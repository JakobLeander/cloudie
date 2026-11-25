
#include <Arduino.h>
#include <ctime>
#include "display/display.h"
#include "network/WiFiManager.h"
#include "network/StateClient.h"
#include "renderer/renderer.h"
#include "utils/IsoTimestamp.h"

namespace
{
    static const uint32_t ATTENTION_AFTER_MS = 10UL * 1000; // Wait this long before shifting to attention animation
    static const uint32_t SLEEP_AFTER_MS = 60UL * 1000;     // Wait this long before shifting to sleep animation

    Renderer renderer(display::WIDTH, display::HEIGHT);
    WiFiManager wifiManager;
    Renderer::Animation animation = Renderer::Animation::Waiting;
    StateClient stateClient;

    uint32_t msSinceLastChange = UINT32_MAX;
    std::string lastRecievedState;

}

void setup()
{
    Serial.begin(115200);
    wifiManager.connect();
    display::begin();
    stateClient.begin();
}

void loop()
{
    // Read the latest state cached by StateClient.
    const std::string state = stateClient.getState();
    const std::string lastUpdatedAt = stateClient.getUpdatedAt();

    msSinceLastChange = UINT32_MAX;
    if (!lastUpdatedAt.empty())
    {
        const uint64_t updatedAtMs = utils::parseIsoTimestampMs(lastUpdatedAt);
        const time_t currentSeconds = std::time(nullptr);

        if (updatedAtMs != 0 && currentSeconds > 0)
        {
            const uint64_t currentMs = static_cast<uint64_t>(currentSeconds) * 1000ULL;
            if (currentMs >= updatedAtMs)
            {
                const uint64_t ageMs = currentMs - updatedAtMs;
                msSinceLastChange = ageMs > UINT32_MAX
                                        ? UINT32_MAX
                                        : static_cast<uint32_t>(ageMs);
            }
        }
    }

    // If there is a recent state change, modify animation
    if (state != lastRecievedState && msSinceLastChange < ATTENTION_AFTER_MS)
    {
        Serial.println(state.c_str());
        lastRecievedState = state;

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

    // Set to idle animation if no state changes for a while
    if (msSinceLastChange >= ATTENTION_AFTER_MS)
    {
        // set to sleep animation if no state changes for longer while
        if (msSinceLastChange >= SLEEP_AFTER_MS)
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