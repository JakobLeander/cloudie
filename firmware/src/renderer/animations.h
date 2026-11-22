// cloudie_anim.h -- parametric "Droplet with arms & feet" Cloudie + excited-animation keyframes.
// Primitives only. Works with any Adafruit_GFX-derived class (GFXcanvas16, Adafruit_SSD1351, ...) and TFT_eSPI sprites.
// For SSD1351: draw into a GFXcanvas16 and push it with drawRGBBitmap (see Cloudie_SSD1351.ino) -> no flicker.
#pragma once
#include <Arduino.h>
#include <math.h>

// If red and blue look swapped on your SSD1351 module, add  #define CLOUDIE_SWAP_RB  before including this file.
#ifdef CLOUDIE_SWAP_RB
constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) { return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3); }
#else
constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }
#endif

struct DropPalette
{
    uint16_t BG, BODY, SHADE, HI, ACC, INK, WHITE, BLUSH, MOUTH, DUST;
};
static const DropPalette PAL_LIME = {
    rgb565(0, 0, 0), rgb565(140, 255, 77), rgb565(53, 184, 42), rgb565(232, 255, 200), rgb565(255, 107, 214),
    rgb565(31, 43, 58), rgb565(255, 255, 255), rgb565(255, 158, 181), rgb565(255, 107, 138), rgb565(181, 221, 245)};

enum Face
{
    FACE_HAPPY,
    FACE_EXCITED,
    FACE_SURPRISED,
    FACE_EFFORT,
    FACE_BLINK,
    FACE_SLEEPY,
    FACE_CALM,
    FACE_LOOK_L,
    FACE_LOOK_R,
    FACE_CALM_BLINK, // relaxed idle faces
    FACE_SAD,
    FACE_SAD_CLOSED,
    FACE_THINK,
    FACE_THINK_L
}; // mood faces
enum Fx
{
    FX_NONE,
    FX_SPEED,
    FX_SPARK,
    FX_DUST,
    FX_BURST,
    FX_ZZZ,
    FX_TEARS,
    FX_THINK,
    FX_ALERT
}; // fxA: tear fall distance / thought dots (0..255) / alert-arc radius

// One pose = everything needed to draw one frame.
struct Pose
{
    int16_t ox, oy;     // shift of the whole character (px). oy<0 = in the air
    int16_t sx, sy;     // body scale in percent of the 30px base radius (80/80 = idle). squash = wide+short, stretch = narrow+tall
    int16_t armL, armR; // arm angle in degrees from "hanging down": 25 relaxed, 90 sideways, 170 straight up
    int16_t lean;       // sideways shift of the drop's tip (px) -> tilt
    int16_t ft;         // extra foot drop (px) while airborne
    int16_t face;       // Face
    int16_t fx;         // Fx
    int16_t fxA;        // effect parameter (burst radius, speed-line length, sparkle set)
};
struct Key
{
    uint16_t t;
    Pose p;
}; // t = ms from sequence start

// ---------------- keyframes ----------------
// 1) EXCITED HOP: crouch -> launch -> apex -> fall -> squash -> rebound -> settle (1.2 s, loops seamlessly)
static const Key SEQ_HOP[] = {
    {0, {0, 0, 90, 68, 45, 45, 0, 0, FACE_EFFORT, FX_NONE, 0}},         // anticipate: squash, arms back/down, "> <" eyes
    {140, {0, -6, 72, 94, 100, 100, 0, 2, FACE_HAPPY, FX_SPEED, 20}},   // launch: stretch, arms swing up, speed lines
    {300, {0, -18, 77, 86, 165, 165, 0, 3, FACE_EXCITED, FX_SPARK, 0}}, // apex: arms up, sparkly eyes, big grin
    {440, {0, -20, 80, 83, 150, 150, 0, 3, FACE_EXCITED, FX_SPARK, 1}}, // float: hang time, sparkles swap
    {580, {0, -8, 74, 91, 120, 120, 0, 4, FACE_SURPRISED, FX_NONE, 0}}, // fall: stretch, legs reach for the ground
    {700, {0, 0, 101, 61, 50, 50, 0, 0, FACE_EFFORT, FX_DUST, 0}},      // impact: biggest squash, dust puffs
    {820, {0, -3, 77, 90, 130, 130, 0, 2, FACE_HAPPY, FX_NONE, 0}},     // rebound: small overshoot stretch
    {1000, {0, 0, 82, 78, 30, 30, 0, 0, FACE_HAPPY, FX_NONE, 0}},       // settle
    {1200, {0, 0, 90, 68, 45, 45, 0, 0, FACE_EFFORT, FX_NONE, 0}},      // = first key, so it loops
};
// 2) HAPPY WIGGLE: "can't stand still" side-to-side with tip tilt and alternating arms (0.6 s)
static const Key SEQ_WIGGLE[] = {
    {0, {-3, 0, 82, 80, 140, 30, -6, 0, FACE_HAPPY, FX_SPARK, 0}},
    {150, {0, -2, 80, 82, 90, 90, 0, 0, FACE_EXCITED, FX_NONE, 0}},
    {300, {3, 0, 82, 80, 30, 140, 6, 0, FACE_HAPPY, FX_SPARK, 1}},
    {450, {0, -2, 80, 82, 90, 90, 0, 0, FACE_EXCITED, FX_NONE, 0}},
    {600, {-3, 0, 82, 80, 140, 30, -6, 0, FACE_HAPPY, FX_SPARK, 0}},
};
// 3) CHEER: arms up, small jump, sparkle ring expands and fades (0.8 s)
static const Key SEQ_CHEER[] = {
    {0, {0, 0, 80, 80, 90, 90, 0, 0, FACE_EXCITED, FX_NONE, 0}},
    {160, {0, -8, 76, 88, 170, 170, 0, 3, FACE_EXCITED, FX_BURST, 14}},
    {320, {0, -12, 78, 85, 175, 175, 0, 3, FACE_EXCITED, FX_BURST, 34}},
    {480, {0, -6, 78, 86, 160, 160, 0, 3, FACE_HAPPY, FX_BURST, 52}},
    {640, {0, 0, 84, 76, 60, 60, 0, 0, FACE_HAPPY, FX_NONE, 0}},
    {800, {0, 0, 80, 80, 90, 90, 0, 0, FACE_EXCITED, FX_NONE, 0}},
};

// ---------------- IDLE sequences (tiny movements only: <= 2 px) ----------------
// 4) IDLE: 8 s loop. Slow breathing (1-2 px), relaxed arm drift, a single blink, a glance left, a glance right, a double blink.
static const Key SEQ_IDLE[] = {
    {0, {0, 0, 80, 80, 25, 25, 0, 0, FACE_CALM, FX_NONE, 0}},    // rest
    {1000, {0, 0, 79, 82, 29, 29, 0, 0, FACE_CALM, FX_NONE, 0}}, // inhale
    {2000, {0, 0, 80, 80, 26, 26, 0, 0, FACE_CALM, FX_NONE, 0}}, // exhale
    {2600, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM, FX_NONE, 0}},
    {2700, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM_BLINK, FX_NONE, 0}}, // blink (~100 ms)
    {2800, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM, FX_NONE, 0}},
    {3000, {0, 0, 79, 82, 29, 29, 0, 0, FACE_CALM, FX_NONE, 0}},     // inhale
    {3400, {-1, 0, 79, 82, 28, 26, -2, 0, FACE_LOOK_L, FX_NONE, 0}}, // glance left: eyes + tip tilt a hair
    {4400, {-1, 0, 80, 80, 27, 25, -2, 0, FACE_LOOK_L, FX_NONE, 0}},
    {4500, {0, 0, 80, 80, 26, 26, 0, 0, FACE_CALM, FX_NONE, 0}},
    {5000, {1, 0, 79, 82, 25, 28, 2, 0, FACE_LOOK_R, FX_NONE, 0}}, // glance right
    {5900, {1, 0, 80, 80, 25, 27, 2, 0, FACE_LOOK_R, FX_NONE, 0}},
    {6000, {0, 0, 80, 80, 26, 26, 0, 0, FACE_CALM, FX_NONE, 0}},
    {6100, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM, FX_NONE, 0}},
    {6200, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM_BLINK, FX_NONE, 0}}, // double blink
    {6300, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM, FX_NONE, 0}},
    {6400, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM_BLINK, FX_NONE, 0}},
    {6500, {0, 0, 80, 81, 27, 27, 0, 0, FACE_CALM, FX_NONE, 0}},
    {7000, {0, 0, 79, 82, 29, 29, 0, 0, FACE_CALM, FX_NONE, 0}}, // inhale
    {8000, {0, 0, 80, 80, 25, 25, 0, 0, FACE_CALM, FX_NONE, 0}}, // = first key, loops
};
// 5) DOZE: after a long idle. Eyes closed, slower/deeper breathing, little "z" bobbing. Pair with dimming (see dimmed()).
static const Key SEQ_DOZE[] = {
    {0, {0, 0, 81, 79, 20, 20, 0, 0, FACE_SLEEPY, FX_ZZZ, 0}},
    {1500, {0, 0, 79, 82, 18, 18, 0, 0, FACE_SLEEPY, FX_ZZZ, 10}},
    {3000, {0, 0, 81, 79, 20, 20, 0, 0, FACE_SLEEPY, FX_ZZZ, 0}},
    {4500, {0, 0, 79, 82, 18, 18, 0, 0, FACE_SLEEPY, FX_ZZZ, 10}},
    {6000, {0, 0, 81, 79, 20, 20, 0, 0, FACE_SLEEPY, FX_ZZZ, 0}},
};

// ---------------- MOOD sequences ----------------
// 6) SAD (3 s loop): slumped body, arms hanging, worried brows, frown, tears that run down, a little sob with eyes shut.
static const Key SEQ_SAD[] = {
    {0, {0, 0, 86, 72, 8, 8, -3, 0, FACE_SAD, FX_TEARS, 0}},
    {900, {0, 0, 84, 75, 12, 12, -4, 0, FACE_SAD, FX_TEARS, 6}},        // slow sigh in
    {1500, {0, 0, 86, 72, 8, 8, -3, 0, FACE_SAD_CLOSED, FX_TEARS, 10}}, // sob: eyes shut + tiny shudder
    {1650, {1, 0, 86, 72, 8, 8, -3, 0, FACE_SAD_CLOSED, FX_TEARS, 12}},
    {1800, {-1, 0, 86, 72, 8, 8, -3, 0, FACE_SAD_CLOSED, FX_TEARS, 14}},
    {1950, {1, 0, 86, 72, 8, 8, -3, 0, FACE_SAD_CLOSED, FX_TEARS, 16}},
    {2100, {0, 0, 86, 72, 8, 8, -3, 0, FACE_SAD, FX_TEARS, 18}},
    {2400, {0, 0, 86, 71, 8, 8, -3, 0, FACE_SAD, FX_TEARS, 20}}, // deepest slump, tear drips off the chin
    {2500, {0, 0, 86, 71, 8, 8, -3, 0, FACE_SAD, FX_NONE, 0}},
    {3000, {0, 0, 86, 72, 8, 8, -3, 0, FACE_SAD, FX_TEARS, 0}}, // new tear starts; = first key, loops
};
// 7) THINKING (3.2 s loop): eyes glance up, one brow raised, finger-tap arm, head tilts, thought cloud fills with dots.
static const Key SEQ_THINK[] = {
    {0, {0, 0, 80, 80, 25, 140, 3, 0, FACE_THINK, FX_THINK, 0}},
    {500, {0, 0, 80, 81, 25, 150, 3, 0, FACE_THINK, FX_THINK, 64}}, // tap, first dot
    {800, {0, 0, 80, 81, 25, 140, 3, 0, FACE_THINK, FX_THINK, 64}},
    {1300, {0, 0, 80, 80, 25, 145, -3, 0, FACE_THINK_L, FX_THINK, 128}}, // glance + tilt other way, 2 dots
    {1800, {0, 0, 80, 81, 25, 150, -3, 0, FACE_THINK_L, FX_THINK, 128}},
    {2100, {0, 0, 80, 81, 25, 140, -3, 0, FACE_THINK_L, FX_THINK, 192}}, // 3 dots
    {2600, {0, 0, 79, 82, 25, 145, 3, 0, FACE_THINK, FX_THINK, 192}},
    {3200, {0, 0, 80, 80, 25, 140, 3, 0, FACE_THINK, FX_THINK, 0}}, // dots clear; = first key, loops
};
// 8) ATTENTION (1.8 s loop): "hey, over here!" two quick hops, big waving arm, "!" mark and vibration arcs.
static const Key SEQ_ATTENTION[] = {
    {0, {0, 0, 80, 80, 25, 100, 0, 0, FACE_EXCITED, FX_ALERT, 0}},
    {150, {0, -7, 76, 88, 25, 165, 0, 2, FACE_EXCITED, FX_ALERT, 6}}, // hop 1
    {300, {0, 0, 84, 76, 25, 120, 0, 0, FACE_EXCITED, FX_ALERT, 0}},
    {450, {0, -7, 76, 88, 25, 165, 0, 2, FACE_EXCITED, FX_ALERT, 6}}, // hop 2
    {600, {0, 0, 84, 76, 25, 120, 0, 0, FACE_SURPRISED, FX_ALERT, 0}},
    {800, {0, 0, 80, 80, 25, 170, 0, 0, FACE_HAPPY, FX_ALERT, 4}}, // big wave
    {1000, {-2, 0, 80, 80, 25, 110, -3, 0, FACE_HAPPY, FX_ALERT, 8}},
    {1200, {2, 0, 80, 80, 25, 170, 3, 0, FACE_HAPPY, FX_ALERT, 4}},
    {1400, {-2, 0, 80, 80, 25, 110, -3, 0, FACE_HAPPY, FX_ALERT, 8}},
    {1600, {0, 0, 80, 80, 25, 140, 0, 0, FACE_EXCITED, FX_ALERT, 0}},
    {1800, {0, 0, 80, 80, 25, 100, 0, 0, FACE_EXCITED, FX_ALERT, 0}}, // = first key, loops
};
static const Pose POSE_IDLE = {0, 0, 80, 80, 25, 25, 0, 0, FACE_HAPPY, FX_NONE, 0};

#define CL_COUNT(a) ((uint8_t)(sizeof(a) / sizeof((a)[0])))

// Interpolated pose at time t (ms). Loops. Smoothstep easing between keys.
inline Pose animPoseAt(const Key *k, uint8_t n, uint32_t t)
{
    uint32_t T = k[n - 1].t;
    if (!T)
        return k[0].p;
    t %= T;
    uint8_t i = 0;
    while (i < n - 2 && t >= k[i + 1].t)
        i++;
    float u = (float)(t - k[i].t) / (float)(k[i + 1].t - k[i].t);
    u = u * u * (3.0f - 2.0f * u);
    const Pose &a = k[i].p, &b = k[i + 1].p;
    auto L = [&](int16_t x, int16_t y)
    { return (int16_t)lroundf(x + (y - x) * u); };
    Pose r;
    r.ox = L(a.ox, b.ox);
    r.oy = L(a.oy, b.oy);
    r.sx = L(a.sx, b.sx);
    r.sy = L(a.sy, b.sy);
    r.armL = L(a.armL, b.armL);
    r.armR = L(a.armR, b.armR);
    r.lean = L(a.lean, b.lean);
    r.ft = L(a.ft, b.ft);
    const Pose &c = (u < 0.5f) ? a : b; // discrete things switch at the midpoint
    r.face = c.face;
    r.fx = c.fx;
    r.fxA = (a.fx == b.fx) ? L(a.fxA, b.fxA) : c.fxA;
    return r;
}

// ---------------- burn-in helpers ----------------
inline uint16_t dimColor(uint16_t c, uint8_t pct)
{
    uint16_t r = ((c >> 11) & 31) * pct / 100, g = ((c >> 5) & 63) * pct / 100, b = (c & 31) * pct / 100;
    return (r << 11) | (g << 5) | b;
}
inline DropPalette dimmed(const DropPalette &P, uint8_t pct)
{ // everything except the background
    DropPalette d = P;
    uint16_t *a = (uint16_t *)&d;
    for (uint8_t i = 1; i < sizeof(DropPalette) / 2; i++)
        a[i] = dimColor(a[i], pct);
    return d;
}
inline void applyDrift(Pose &p, uint32_t nowMs)
{ // slow, invisible wander: +-3 px / +-2 px
    p.ox += (int16_t)lroundf(3 * sinf(nowMs / 60000.0f));
    p.oy += (int16_t)lroundf(2 * sinf(nowMs / 47000.0f + 1.3f));
}

// ---------------- primitive helpers ----------------
template <class G>
void fillEll(G &g, int cx, int cy, int rx, int ry, uint16_t c)
{
    if (ry <= 0 || rx <= 0)
        return;
    for (int y = -ry; y <= ry; y++)
    {
        int w = (int)(rx * sqrtf(1.0f - (float)(y * y) / (float)(ry * ry)) + 0.5f);
        g.drawFastHLine(cx - w, cy + y, 2 * w + 1, c);
    }
}
template <class G>
void thickLine(G &g, int x0, int y0, int x1, int y1, uint16_t c)
{
    int dx = x1 - x0, dy = y1 - y0, n = max(abs(dx), abs(dy));
    if (!n)
    {
        g.fillCircle(x0, y0, 1, c);
        return;
    }
    for (int i = 0; i <= n; i++)
        g.fillCircle(x0 + dx * i / n, y0 + dy * i / n, 1, c);
}
template <class G>
void thickArc(G &g, int cx, int cy, int r, int a0, int a1, uint16_t c)
{
    for (int a = a0; a <= a1; a += 2)
    {
        float t = a * 0.0174533f;
        g.fillCircle(cx + (int)lroundf(r * cosf(t)), cy - (int)lroundf(r * sinf(t)), 1, c);
    }
}
template <class G>
void plusMark(G &g, int x, int y, int s, uint16_t c)
{
    thickLine(g, x - s, y, x + s, y, c);
    thickLine(g, x, y - s, x, y + s, c);
}

// Drop = ellipse + triangle from the (possibly leaning) tip to the tangent points.
template <class G>
void dropShape(G &g, int cx, int cy, int rx, int ry, int ax, int ay, uint16_t c)
{
    fillEll(g, cx, cy, rx, ry, c);
    if (ry <= 0 || rx <= 0)
        return;
    float k = (float)rx / (float)ry, Y0 = cy * k, AY = ay * k;
    float dx = cx - ax, dy = Y0 - AY, d = sqrtf(dx * dx + dy * dy);
    if (d <= rx + 1)
        return;
    float phi = atan2f(dy, dx), th = asinf(rx / d), tl = sqrtf(d * d - (float)rx * rx);
    int x1 = (int)lroundf(ax + tl * cosf(phi - th)), y1 = (int)lroundf((AY + tl * sinf(phi - th)) / k);
    int x2 = (int)lroundf(ax + tl * cosf(phi + th)), y2 = (int)lroundf((AY + tl * sinf(phi + th)) / k);
    g.fillTriangle(ax, ay, x1, y1, x2, y2, c);
}

// ---------------- the character ----------------
template <class G>
void drawCloudiePose(G &g, const Pose &p, const DropPalette &P = PAL_LIME, bool clear = true)
{
    if (clear)
        g.fillScreen(P.BG);
    const int bx = 64 + p.ox, bottom = 112 + p.oy;
    const int rx = (int)lroundf(30 * p.sx / 100.0f), ry = (int)lroundf(30 * p.sy / 100.0f);
    const int cy = bottom - ry, ay = cy - 2 * ry, ax = bx + p.lean;
    auto FX = [&](int x)
    { return bx + (int)lroundf((x - 64) * p.sx / 100.0f); }; // base-coordinate -> screen
    auto FY = [&](int y)
    { return cy + (int)lroundf((y - 82) * p.sy / 100.0f); };

    // arms (behind body)
    auto limb = [&](int x0, int y0, int x1, int y1)
    {
        int dx = x1 - x0, dy = y1 - y0, n = max(abs(dx), abs(dy));
        if (!n)
            n = 1;
        for (int i = 0; i <= n; i++)
            g.fillCircle(x0 + dx * i / n, y0 + dy * i / n, 4, P.SHADE);
        for (int i = 0; i <= n; i++)
            g.fillCircle(x0 + dx * i / n, y0 + dy * i / n, 3, P.BODY);
        g.fillCircle(x1, y1, 6, P.SHADE);
        g.fillCircle(x1, y1, 5, P.BODY);
    };
    auto arm = [&](int side, int deg)
    {
        float a = deg * 0.0174533f;
        int x0 = bx + side * (rx - 2), y0 = cy + 10;
        limb(x0, y0, x0 + (int)lroundf(side * 17 * sinf(a)), y0 + (int)lroundf(17 * cosf(a)));
    };
    arm(-1, p.armL);
    arm(+1, p.armR);

    // feet (behind body)
    const int fyc = bottom + 2 + p.ft, fo = (int)lroundf(14 * p.sx / 100.0f) + (p.sx > 100 ? (p.sx - 100) / 2 : 0) + 4;
    for (int s = -1; s <= 1; s += 2)
    {
        fillEll(g, bx + s * fo, fyc, 11, 6, P.SHADE);
        fillEll(g, bx + s * fo, fyc, 9, 4, P.BODY);
    }

    // body: outline, fill, shine, blush
    dropShape(g, bx, cy, rx + 2, ry + 2, ax, ay - 6, P.SHADE);
    dropShape(g, bx, cy, rx, ry, ax, ay, P.BODY);
    fillEll(g, FX(47), FY(68), 3, 7, P.HI);
    g.fillCircle(FX(49), FY(57), 2, P.HI);
    fillEll(g, FX(42), FY(92), 5, 3, P.BLUSH);
    fillEll(g, FX(86), FY(92), 5, 3, P.BLUSH);

    // face
    auto arcs = [&](int ey, int a0, int a1)
    { thickArc(g,FX(52),FY(ey),6,a0,a1,P.INK); thickArc(g,FX(76),FY(ey),6,a0,a1,P.INK); };
    auto mouthD = [&](int my, int mrx, int mry)
    {
        fillEll(g, FX(64), FY(my), mrx, mry, P.INK);
        fillEll(g, FX(64), FY(my), mrx - 2, mry - 2, P.MOUTH);
        g.fillRect(FX(64) - mrx - 1, FY(my) - mry, 2 * mrx + 3, mry, P.BODY);
    };
    switch (p.face)
    {
    case FACE_HAPPY:
        arcs(86, 30, 150);
        mouthD(95, 8, 6);
        break;
    case FACE_BLINK:
        arcs(81, 210, 330);
        mouthD(95, 8, 6);
        break;
    case FACE_SLEEPY:
        arcs(81, 210, 330);
        fillEll(g, FX(64), FY(97), 3, 4, P.INK);
        fillEll(g, FX(64), FY(97), 2, 3, P.MOUTH);
        break;
    case FACE_SURPRISED:
        for (int s = 0; s < 2; s++)
        {
            int x = FX(s ? 76 : 52), y = FY(85);
            g.fillCircle(x, y, 6, P.INK);
            g.fillCircle(x, y, 4, P.WHITE);
            g.fillCircle(x, y + 1, 2, P.INK);
        }
        fillEll(g, FX(64), FY(98), 4, 5, P.INK);
        fillEll(g, FX(64), FY(98), 3, 4, P.MOUTH);
        break;
    case FACE_EXCITED:
        for (int s = 0; s < 2; s++)
        {
            int x = FX(s ? 76 : 52), y = FY(84);
            g.fillCircle(x, y, 6, P.INK);
            g.fillCircle(x - 2, y - 2, 2, P.WHITE);
            g.fillCircle(x + 2, y + 2, 1, P.WHITE);
        }
        mouthD(98, 10, 7);
        break;
    case FACE_CALM:
    case FACE_LOOK_L:
    case FACE_LOOK_R:
    {
        const int sh = (p.face == FACE_LOOK_L) ? -3 : (p.face == FACE_LOOK_R) ? 3
                                                                              : 0;
        for (int s = 0; s < 2; s++)
        {
            int x = FX(s ? 76 : 52) + sh, y = FY(85);
            g.fillCircle(x, y, 5, P.INK);
            g.fillCircle(x - 1, y - 2, 2, P.WHITE);
            g.fillCircle(x + 2, y + 2, 1, P.WHITE);
        }
        thickArc(g, FX(64) + sh / 2, FY(91), 6, 235, 305, P.INK);
        break;
    }
    case FACE_CALM_BLINK:
        arcs(80, 210, 330);
        thickArc(g, FX(64), FY(91), 6, 235, 305, P.INK);
        break;
    case FACE_SAD:
    case FACE_SAD_CLOSED:
        if (p.face == FACE_SAD)
        {
            for (int s = 0; s < 2; s++)
            {
                int x = FX(s ? 76 : 52), y = FY(86);
                g.fillCircle(x, y, 6, P.INK);
                g.fillCircle(x - 2, y - 2, 2, P.WHITE);
                g.fillCircle(x + 2, y + 2, 1, P.WHITE);
            }
        }
        else
            arcs(80, 210, 330);
        thickLine(g, FX(44), FY(75), FX(56), FY(70), P.INK);
        thickLine(g, FX(72), FY(70), FX(84), FY(75), P.INK); // worried brows
        thickArc(g, FX(64), FY(101), 7, 55, 125, P.INK);
        break; // frown
    case FACE_THINK:
    case FACE_THINK_L:
    {
        const int sh = (p.face == FACE_THINK) ? 3 : -3;
        for (int s = 0; s < 2; s++)
        {
            int x = FX(s ? 76 : 52) + sh, y = FY(83);
            g.fillCircle(x, y, 5, P.INK);
            g.fillCircle(x - 1, y - 2, 2, P.WHITE);
            g.fillCircle(x + 2, y + 1, 1, P.WHITE);
        }
        if (p.face == FACE_THINK)
        {
            thickLine(g, FX(45), FY(73), FX(57), FY(74), P.INK);
            thickLine(g, FX(70), FY(69), FX(82), FY(66), P.INK); // right brow raised
            thickLine(g, FX(60), FY(95), FX(70), FY(93), P.INK);
        }
        else
        {
            thickLine(g, FX(46), FY(66), FX(58), FY(69), P.INK);
            thickLine(g, FX(71), FY(74), FX(83), FY(73), P.INK); // left brow raised
            thickLine(g, FX(58), FY(93), FX(68), FY(95), P.INK);
        }
        break;
    }
    case FACE_EFFORT:
        thickLine(g, FX(47), FY(81), FX(55), FY(85), P.INK);
        thickLine(g, FX(55), FY(85), FX(47), FY(89), P.INK);
        thickLine(g, FX(81), FY(81), FX(73), FY(85), P.INK);
        thickLine(g, FX(73), FY(85), FX(81), FY(89), P.INK);
        thickArc(g, FX(64), FY(91), 8, 240, 300, P.INK);
        break;
    }

    // effects
    switch (p.fx)
    {
    case FX_SPEED:
        for (int dx = -18; dx <= 18; dx += 18)
            thickLine(g, bx + dx, bottom + 12, bx + dx, bottom + 12 + 4 + p.fxA / 8, P.ACC);
        break;
    case FX_SPARK:
    {
        static const int8_t S0[4][2] = {{-46, -34}, {44, -44}, {-42, 16}, {50, 6}};
        static const int8_t S1[4][2] = {{-50, -10}, {40, -30}, {-34, 30}, {48, 22}};
        const int8_t (*S)[2] = (p.fxA & 1) ? S1 : S0;
        for (int i = 0; i < 4; i++)
            plusMark(g, bx + S[i][0], cy + S[i][1], i & 1 ? 2 : 3, P.ACC);
        break;
    }
    case FX_DUST:
        for (int s = -1; s <= 1; s += 2)
        {
            g.fillCircle(bx + s * (rx + 8), bottom + 3, 6, P.DUST);
            g.fillCircle(bx + s * (rx + 8), bottom + 3, 5, P.WHITE);
            g.fillCircle(bx + s * (rx + 17), bottom, 4, P.DUST);
            g.fillCircle(bx + s * (rx + 17), bottom, 3, P.WHITE);
            plusMark(g, bx + s * 34, cy - 32, 2, P.ACC);
        }
        break;
    case FX_ZZZ:
    {
        int zx = bx + rx + 8, zy = cy - ry / 2 - p.fxA / 3;
        thickLine(g, zx - 4, zy, zx + 4, zy, P.ACC);
        thickLine(g, zx + 4, zy, zx - 4, zy + 8, P.ACC);
        thickLine(g, zx - 4, zy + 8, zx + 4, zy + 8, P.ACC);
        zx += 9;
        zy -= 13;
        thickLine(g, zx - 3, zy, zx + 3, zy, P.ACC);
        thickLine(g, zx + 3, zy, zx - 3, zy + 6, P.ACC);
        thickLine(g, zx - 3, zy + 6, zx + 3, zy + 6, P.ACC);
        break;
    }
    case FX_TEARS:
        for (int i = 0; i < 2; i++)
        {
            int fall = p.fxA - i * 6;
            if (fall < 0)
                continue; // second tear lags behind
            int x = FX(i ? 84 : 44) + (i ? 2 : -2), y = FY(92) + fall;
            g.fillCircle(x, y, 2, P.DUST);
            g.fillTriangle(x, y - 6, x - 2, y - 1, x + 2, y - 1, P.DUST);
            g.fillRect(x - 1, y - 1, 1, 1, P.WHITE);
        }
        break;
    case FX_THINK:
    {
        const int8_t B[4][3] = {{-9, 2, 8}, {0, -3, 9}, {9, 2, 8}, {0, 4, 8}}; // thought cloud = 4 overlapping circles
        const int cxb = bx + 32, cyb = bottom - 84, dots = p.fxA / 64;
        g.fillCircle(bx + 16, bottom - 56, 2, P.DUST);
        g.fillCircle(bx + 16, bottom - 56, 1, P.WHITE); // trail of little bubbles
        g.fillCircle(bx + 21, bottom - 64, 3, P.DUST);
        g.fillCircle(bx + 21, bottom - 64, 2, P.WHITE);
        for (int i = 0; i < 4; i++)
            g.fillCircle(cxb + B[i][0], cyb + B[i][1], B[i][2] + 1, P.DUST);
        for (int i = 0; i < 4; i++)
            g.fillCircle(cxb + B[i][0], cyb + B[i][1], B[i][2], P.WHITE);
        for (int i = 0; i < 3; i++)
            g.fillCircle(cxb - 8 + i * 8, cyb + 2, 2, i < dots ? P.INK : P.DUST);
        break;
    }
    case FX_ALERT:
    {
        const int ex = bx + 30, ey = bottom - 98, rad = 38 + p.fxA;
        thickLine(g, ex, ey, ex, ey + 13, P.ACC);
        g.fillCircle(ex, ey + 20, 2, P.ACC); // "!"
        thickArc(g, bx, cy - 6, rad, 145, 215, P.ACC);
        thickArc(g, bx, cy - 6, rad, -35, 35, P.ACC); // vibration arcs
        break;
    }
    case FX_BURST:
        for (int i = 0; i < 6; i++)
        {
            float a = (i * 60 + 15) * 0.0174533f;
            int rad = 34 + p.fxA / 2; // ring starts clear of the face
            plusMark(g, bx + (int)lroundf(rad * cosf(a)), cy - 10 + (int)lroundf(rad * sinf(a)), max(1, 3 - p.fxA / 20), P.ACC);
        }
        break;
    }
}
