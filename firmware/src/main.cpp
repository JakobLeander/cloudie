
#include <Arduino.h>
#include "display/display.h"
#include "renderer/renderer.h"

namespace
{
    constexpr unsigned long faceChangeIntervalMs = 5000;
    unsigned long lastFaceChangeMs = 0;
    using FaceRenderer = void (*)();
    constexpr FaceRenderer faces[] = {
        renderer::drawHappyFace,
        renderer::drawSadFace,
        renderer::drawNeutralFace,
        renderer::drawCuriousFace,
        renderer::drawThinkingFace,
    };
    constexpr size_t faceCount = sizeof(faces) / sizeof(faces[0]);
    size_t currentFace = 0;
}

void setup()
{
    display::begin();
    faces[currentFace]();
    lastFaceChangeMs = millis();
}

void loop()
{
    const unsigned long now = millis();
    if (now - lastFaceChangeMs >= faceChangeIntervalMs)
    {
        currentFace = (currentFace + 1) % faceCount;
        faces[currentFace]();
        lastFaceChangeMs = now;
    }
}