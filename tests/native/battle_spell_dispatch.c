#include <persona/btlp/battle.h>
#include <persona/btlp/object.h>
#include <persona/btlp/round.h>
#include <persona/btlp/spellfx.h>

extern int printf(const char *, ...);

SVECTOR g_btl_cam_rot;
short g_btl_intro_dist[1];
u_char g_btl_fx_move;

static BtlObj *received, *rearmed;
static BtlSeqStep *received_script;
static unsigned calls, handler, rearms;

static void first(BtlObj *o)
{
    received = o;
    calls++;
    handler = 1;
}

static void second(BtlObj *o)
{
    received = o;
    calls++;
    handler = 2;
}

BtlSpellFx g_btl_spell_fx[] = {
    {0, 0, 0, 0},
    {0, first, 0, 0},
    {0, second, 0, 0}
};

int BtlObjSetScript(BtlObj *o, BtlSeqStep *script)
{
    rearmed = o;
    received_script = script;
    rearms++;
    return 0;
}

/* Link the real effect dispatcher. Distinct objects and callbacks expose a
   missing argument even when incidental register contents hide it elsewhere. */
int main(void)
{
    static const short rotations[][3] = {
        {0, 0, 0}, {-32768, 32767, -1234}, {123, -456, 789}
    };
    static const u_long scripts[2][2] = {{11, 22}, {33, 44}};
    unsigned move, motion, flags, slot, rotation, script, cases = 0;
    BtlObj objects[2], *o;
    unsigned long attr;
    int should_call, should_rearm;

    for (move = 0; move < 3; move++)
    for (motion = 0; motion < 3; motion++)
    for (flags = 0; flags < 4; flags++)
    for (slot = 0; slot < 2; slot++)
    for (rotation = 0; rotation < 3; rotation++)
    for (script = 0; script < 2; script++) {
        BtlObj empty = {0};
        objects[0] = objects[1] = empty;
        o = &objects[slot];
        attr = BTL_OBJ_HIDDEN;
        if (flags & 1) attr |= BTL_OBJ_TRACKING;
        if (flags & 2) attr |= BTL_OBJ_ANIMATING;
        o->attr = attr;
        o->motion = motion;
        o->phase = 17;
        o->rot.vx = 111;
        o->rot.vy = 222;
        o->rot.vz = 333;
        o->rot.pad = 444;
        o->next_script = scripts[script];
        g_btl_fx_move = move;
        g_btl_cam_rot.vx = rotations[rotation][0];
        g_btl_cam_rot.vy = rotations[rotation][1];
        g_btl_cam_rot.vz = 555; /* Tracking uses intro distance for Z. */
        g_btl_intro_dist[0] = rotations[rotation][2];
        received = rearmed = 0;
        received_script = 0;
        calls = handler = rearms = 0;
        should_call = !(flags & 1) && move != 0 && motion == 2;
        should_rearm = (flags & 1) && !(flags & 2);

        BtlFxObjTick(o);

        if (calls != should_call || received != (should_call ? o : 0)
            || handler != (should_call ? move : 0)
            || rearms != should_rearm || rearmed != (should_rearm ? o : 0)
            || received_script != (should_rearm ? (BtlSeqStep *)scripts[script] : 0)
            || o->rot.vx != ((flags & 1) ? rotations[rotation][0] : 111)
            || o->rot.vy != ((flags & 1) ? rotations[rotation][1] : 222)
            || o->rot.vz != ((flags & 1) ? rotations[rotation][2] : 333)
            || o->rot.pad != 444 || o->phase != 17 || o->attr != attr
            || o->motion != motion || o->next_script != scripts[script]
            || objects[1 - slot].attr != 0 || objects[1 - slot].phase != 0
            || objects[1 - slot].rot.vx != 0 || objects[1 - slot].rot.vy != 0
            || objects[1 - slot].rot.vz != 0) {
            printf("spell dispatch failed: move=%u motion=%u flags=%u slot=%u rotation=%u script=%u\n",
                   move, motion, flags, slot, rotation, script);
            return 1;
        }
        cases++;
    }
    printf("battle spell dispatcher: %u cases passed\n", cases);
    return 0;
}
