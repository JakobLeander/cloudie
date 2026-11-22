#include "renderer.h"
#include "animations.h"
#include <math.h>
#include <stddef.h>

Renderer::Renderer(uint16_t width, uint16_t height) : m_canvas(width, height)
{
}

struct Step
{
    const Key *keys;
    uint8_t n;
    uint8_t loops;
};
static const Step EXCITED[] = {
    {SEQ_HOP, CL_COUNT(SEQ_HOP), 2},
    {SEQ_CHEER, CL_COUNT(SEQ_CHEER), 1},
    {SEQ_WIGGLE, CL_COUNT(SEQ_WIGGLE), 2},
};

void Renderer::setAnimation(Animation animation)
{
    m_animation = animation;
    m_stepStart = millis();
    m_step = 0;
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
    case Animation::Happy:
    {
        const Step &s = EXCITED[m_step];
        uint32_t len = s.keys[s.n - 1].t;
        uint32_t t = m_now - m_stepStart;
        uint32_t duration = len * s.loops;

        if (t >= duration)
        {
            m_stepStart = m_now;
            m_step = (m_step + 1) % CL_COUNT(EXCITED);
            t = 0;
        }

        const Step &current = EXCITED[m_step];
        p = animPoseAt(current.keys, current.n, t);
    }
    break;
    case Animation::Sleep:
        p = animPoseAt(SEQ_DOZE, CL_COUNT(SEQ_DOZE), m_now - m_stepStart);
        break;
    case Animation::Waiting:
        p = animPoseAt(SEQ_IDLE, CL_COUNT(SEQ_IDLE), m_now - m_stepStart);
        break;
    case Animation::Sad:
        p = animPoseAt(SEQ_SAD, CL_COUNT(SEQ_SAD), m_now - m_stepStart);
        break;

    case Animation::Think:
        p = animPoseAt(SEQ_THINK, CL_COUNT(SEQ_THINK), m_now - m_stepStart);
        break;

    case Animation::Attention:
        p = animPoseAt(SEQ_ATTENTION, CL_COUNT(SEQ_ATTENTION), m_now - m_stepStart);
        break;

    default:
        p = animPoseAt(SEQ_IDLE, CL_COUNT(SEQ_IDLE), m_now - m_stepStart);
        break;
    }

    applyDrift(p, m_now); // slow +-3 px wander against burn-in
    drawCloudiePose(m_canvas, p);
    return m_canvas.getBuffer();
}