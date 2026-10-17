#include <persona/casino/casino.h>
#include <libsnd.h>

typedef struct CasinoRotationResponse
{
    u_char pad00[0x50];
    double angle;
    u_char pad58[0x60];
    double velocity;
    u_char padC0[0x20];
    s8 mode;
    u_char padE1[2];
    s8 band;
    s8 previous_band;
} CasinoRotationResponse;

void func_80091D0C(CasinoRotationResponse *value)
{
    u_int angle;

    angle = (int)value->angle % 360;
    if (angle < 90U)
    {
        value->band = 0;
    }
    else if (angle - 180 < 90U)
    {
        value->band = 1;
    }
    if (value->previous_band != value->band && value->mode > 0)
    {
        SsSeqStop(g_casino_seqs[9]);
        SsSeqPlay(g_casino_seqs[9], 1, 1);
        if (value->band != 0)
        {
            value->velocity = 0.777 * value->velocity;
        }
        value->previous_band = value->band;
    }
}
