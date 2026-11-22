#include "renderer.h"
#include "animations.h"
#include <math.h>
#include <stddef.h>

Renderer::Renderer(uint16_t width, uint16_t height) : m_canvas(width, height)
{
}

void Renderer::setAnimation(Animation animation)
{
    m_animation = animation;
    m_stepStart = millis();
}

const uint16_t *Renderer::render()
{
    m_now = millis();
    if ((int32_t)(m_now - m_nextFrame) < 0)
        return nullptr; // frame pacing
    m_nextFrame = m_now + FRAME_MS;
    m_idleFor = m_now - m_lastActivity;

    Pose p;
    switch (m_animation)
    {
    case Animation::Sleep:
        p = animPoseAt(SEQ_DOZE, CL_COUNT(SEQ_DOZE), m_now - m_stepStart);
        break;
    case Animation::Attentive:
    default:
        p = animPoseAt(SEQ_IDLE, CL_COUNT(SEQ_IDLE), m_now - m_stepStart);
        break;
    }

    applyDrift(p, m_now); // slow +-3 px wander against burn-in
    drawCloudiePose(m_canvas, p);
    return m_canvas.getBuffer();
}