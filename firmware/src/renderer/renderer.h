#pragma once

#include <Adafruit_GFX.h>

class Renderer
{
public:
    enum class Animation
    {
        Sleep,
        Attentive,
        Happy
    };

    Renderer(uint16_t width, uint16_t height);

    void setAnimation(Animation animation);
    const uint16_t *render();

private:
    // ---- behaviour ----
    static const uint32_t DOZE_AFTER_MS = 2UL * 60 * 1000;   // idle this long -> eyes close, OLED dims
    static const uint32_t BLANK_AFTER_MS = 20UL * 60 * 1000; // idle this long -> panel off (0 = never)
    static const uint8_t CONTRAST_AWAKE = 15;                // SSD1351 master contrast, 0..15 (power-on default 15)
    static const uint8_t CONTRAST_DOZE = 7;
    static const uint32_t FRAME_MS = 33; // ~30 fps

    GFXcanvas16 m_canvas;
    uint32_t m_now = 0;
    uint32_t m_stepStart = 0;
    uint32_t m_step = 0;
    uint32_t m_lastActivity = 0;
    uint32_t m_nextFrame = 0;
    uint32_t m_idleFor = 0;
    Animation m_animation = Animation::Attentive;
};
