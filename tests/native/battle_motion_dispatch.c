#include <persona/btlp/object.h>

extern int printf(const char *, ...);
static BtlObj *received;
static unsigned calls, handler;

static void first(BtlObj *o)
{
    received = o;
    calls++;
    handler = 1;
    o->phase = 13;
}

static void second(BtlObj *o)
{
    received = o;
    calls++;
    handler = 2;
    o->phase = 17;
}

void (*g_btl_persona_motion[])(BtlObj *) = {0, first, second};
void (*g_btl_enemy_motion[])(BtlObj *) = {0, first, second};
void (*g_btl_member_motion[])(BtlObj *) = {0, first, second};
long g_btl_persona_hover[16];
void BtlTalkFxObjTick(BtlObj *o) { (void)o; }
void BtlFxObjTick(BtlObj *o) { (void)o; }

int main(void)
{
    void (*dispatch[])(BtlObj *) = {BtlTickPersona, BtlTickEnemy, BtlTickMember};
    unsigned group, motion, held, slot, i, cases = 0;
    BtlObj objects[2], *o;
    int should_call;

    for (group = 0; group < 3; group++)
    for (motion = 0; motion < 3; motion++)
    for (held = 0; held < 2; held++)
    for (slot = 0; slot < 2; slot++) {
        BtlObj empty = {0};
        for (i = 0; i < 2; i++) objects[i] = empty;
        o = &objects[slot];
        o->motion = motion;
        o->attr = held ? BTL_OBJ_HELD : 0;
        o->phase = 9;
        o->z = o->z2 = 123;
        received = 0;
        calls = handler = 0;
        should_call = motion != 0 && (group == 0 || !held);
        dispatch[group](o);
        if (calls != should_call || received != (should_call ? o : 0)
            || handler != (should_call ? motion : 0)
            || o->phase != (should_call ? (motion == 1 ? 13 : 17) : 0)
            || o->z != 123 || objects[1 - slot].phase != 0) {
            printf("motion dispatch failed: group=%u motion=%u held=%u slot=%u\n",
                   group, motion, held, slot);
            return 1;
        }
        cases++;
    }
    printf("battle motion dispatchers: %u cases passed\n", cases);
    return 0;
}
