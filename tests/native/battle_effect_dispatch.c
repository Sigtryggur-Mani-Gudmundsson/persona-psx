#include <persona/btlp/actor.h>
#include <persona/btlp/battle.h>
#include <persona/btlp/object.h>

extern int printf(const char *, ...);

BtlActor g_btl_actors[1];
const int g_btl_wave_sin[] = {65536, -65536};
const int g_btl_wave_cos[] = {-32768, 32768};
long g_btl_persona_hover[16];
void (*g_btl_persona_motion[1])(BtlObj *o);
void (*g_btl_enemy_motion[1])(BtlObj *o);
void (*g_btl_member_motion[1])(BtlObj *o);
static BtlObj *freed, *ordinary;

int BtlObjFree(BtlObj *o) { freed = o; return 0; }
void BtlFxObjTick(BtlObj *o) { ordinary = o; }

/* Link the real dispatcher and negotiation tick. Their shared prototypes
   reject a stale no-argument call before accidental register state can make
   a native run appear to work. */
int main(void)
{
    unsigned kind, phase, timer, flags, selected, angle, cases = 0;
    BtlObj objects[2], *o;
    unsigned long attr;
    int waits, expect_free, expect_phase;
    long expect_x, expect_y;

    for (kind = 0; kind < 256; kind++)
    for (phase = 0; phase < 3; phase++)
    for (timer = 0; timer < 3; timer++)
    for (flags = 0; flags < 4; flags++)
    for (selected = 0; selected < 2; selected++)
    for (angle = 0; angle < 2; angle++) {
        BtlObj empty = {0};
        objects[0] = objects[1] = empty;
        o = &objects[selected];
        o->kind = kind;
        o->phase = phase;
        o->timer = (int)timer - 1;
        o->angle = angle;
        o->shift_x = 13;
        o->shift = -19;
        attr = BTL_OBJ_HIDDEN | BTL_OBJ_STATIC;
        if (flags & 1) attr |= BTL_OBJ_ATTR_2000;
        if (flags & 2) attr |= BTL_OBJ_ANIMATING;
        o->attr = attr;
        freed = ordinary = 0;
        expect_free = 0;
        expect_phase = phase;
        expect_x = 13;
        expect_y = -19;
        waits = kind == 0xAF || kind == 0xBD || kind == 0xCA
             || kind == 0xD3 || kind == 0xD4 || kind == 0xDE;
        if (flags & 1) {
            if (waits) {
                if (phase == 0 && timer == 1) {
                    attr &= ~(BTL_OBJ_HIDDEN | BTL_OBJ_STATIC);
                    expect_phase = 1;
                }
                expect_free = phase == 1 && !(flags & 2);
            } else if (kind == 0xBB) {
                if (phase == 0) {
                    expect_x += g_btl_wave_sin[angle] * 4;
                    expect_y += g_btl_wave_cos[angle] * 4;
                    expect_free = timer == 1;
                }
            } else {
                expect_free = !(flags & 2);
            }
        }
        BtlTickEffects(o);
        if (ordinary != ((flags & 1) ? 0 : o)
            || freed != (expect_free ? o : 0)
            || o->attr != attr || o->phase != expect_phase
            || o->shift_x != expect_x || o->shift != expect_y
            || o->kind != kind || o->timer != (int)timer - 1
            || objects[1 - selected].attr != 0
            || objects[1 - selected].phase != 0
            || objects[1 - selected].shift_x != 0
            || objects[1 - selected].shift != 0) {
            printf("effect dispatch failed: kind=%u phase=%u timer=%u flags=%u slot=%u\n",
                   kind, phase, timer, flags, selected);
            return 1;
        }
        cases++;
    }
    printf("battle effect dispatcher: %u cases passed\n", cases);
    return 0;
}
