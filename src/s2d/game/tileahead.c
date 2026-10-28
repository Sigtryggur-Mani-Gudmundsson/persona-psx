/* Persona 1 (JP) - the map cell `distance` tiles ahead of an actor. */
#include <decomp/types.h>

typedef struct {
    u8 colour, enabled, pad2[2];
    union {
        u16 packed;
        u8 xy[2];
    } pos;
    u16 previous;
    u8 speed, fracX, fracY;
    s8 dx, dy;
    u8 direction;
} MapActor;

/* The step along the facing's axis: north and west go down, east and south up. */
typedef struct {
    s8 step[4];
} FacingSteps;
extern FacingSteps D_80064FDC;

/* Writes the cell `distance` tiles ahead in the actor's facing to out[0]
   (x) and out[1] (y) - even facings move y, odd ones x - wrapped onto the
   200-cell map, and returns out. */
s16 *func_80094E68(MapActor *actor, s16 distance, s16 *out)
{
    FacingSteps steps = D_80064FDC;
    s16 *axis;

    out[0] = actor->pos.xy[0];
    out[1] = actor->pos.xy[1];
    axis = &out[1 - (actor->direction & 1)];
    *axis += steps.step[(s8)actor->direction] * distance;
    if (*axis < 0) {
        *axis += 200;
    }
    if (*axis >= 200) {
        *axis -= 200;
    }
    return out;
}
