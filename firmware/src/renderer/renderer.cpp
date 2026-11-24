#include "Renderer.h"
#include "Animations.h"
#include <math.h>
#include <stddef.h>

Renderer::Renderer(uint16_t width, uint16_t height) : m_canvas(width, height)
{
}

void Renderer::setAnimation(Animation animation)
{
    m_nextAnimation = animation;
}

const uint16_t *Renderer::render()
{
    m_now = millis();
    if ((int32_t)(m_now - m_nextFrame) < 0)
        return nullptr; // frame pacing
    m_nextFrame = m_now + FRAME_MS;

    bool animationComplete = false;

    Pose p;
    switch (m_animation)
    {
    case Animation::Happy:
        p = animPoseAt(SEQ_HOP, CL_COUNT(SEQ_HOP), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_HOP[CL_COUNT(SEQ_HOP) - 1].t)
        {
            animationComplete = true;
        }
        break;

    case Animation::Sleep:
        p = animPoseAt(SEQ_DOZE, CL_COUNT(SEQ_DOZE), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_DOZE[CL_COUNT(SEQ_DOZE) - 1].t)
        {
            animationComplete = true;
        }
        break;

    case Animation::Waiting:
        p = animPoseAt(SEQ_IDLE, CL_COUNT(SEQ_IDLE), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_IDLE[CL_COUNT(SEQ_IDLE) - 1].t)
        {
            animationComplete = true;
        }
        break;
    case Animation::Sad:
        p = animPoseAt(SEQ_SAD, CL_COUNT(SEQ_SAD), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_SAD[CL_COUNT(SEQ_SAD) - 1].t)
        {
            animationComplete = true;
        }
        break;

    case Animation::Think:
        p = animPoseAt(SEQ_THINK, CL_COUNT(SEQ_THINK), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_THINK[CL_COUNT(SEQ_THINK) - 1].t)
        {
            animationComplete = true;
        }
        break;

    case Animation::Attention:
        p = animPoseAt(SEQ_ATTENTION, CL_COUNT(SEQ_ATTENTION), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_ATTENTION[CL_COUNT(SEQ_ATTENTION) - 1].t)
        {
            animationComplete = true;
        }
        break;

    default:
        p = animPoseAt(SEQ_IDLE, CL_COUNT(SEQ_IDLE), m_now - m_stepStart);
        if (m_now - m_stepStart >= SEQ_IDLE[CL_COUNT(SEQ_IDLE) - 1].t)
        {
            animationComplete = true;
        }
        break;
    }

    // slow +-3 px wander against burn-in
    applyDrift(p, m_now);

    drawCloudiePose(m_canvas, p);

    // If the current animation is complete and there's a next animation queued, switch to it
    if (animationComplete && m_nextAnimation != m_animation)
    {
        m_animation = m_nextAnimation;
    }

    return m_canvas.getBuffer();
}