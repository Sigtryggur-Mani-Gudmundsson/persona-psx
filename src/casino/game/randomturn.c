/* Choose a quarter-turn for the current and previous preview transforms. */
#include <persona/casino/casino.h>

extern int rand(void);

void func_80093B7C(CasinoXform *p)
{
    int sample;
    short angle;
    int roll;
    int value;

    value = rand();
    roll = value >> 2;
    sample = value;
    if (sample < 0) {
        /* The equal arms retain cc1's original value-copy lifetime. Folding
           them changes the source register of the first arithmetic shift. */
        if (roll) {
            roll = (sample + 3) >> 2;
        } else {
            roll = (sample + 3) >> 2;
        }
    }
    roll = sample - roll * 4;

    if (roll == 1) goto one;
    if (roll < 2) goto zero;
    if (roll == 2) goto two;
    if (roll == 3) goto three;
    p[1].rot.vy = 0;
    goto zero_second;
one:
    angle = 1024;
    goto set;
two:
    angle = 2048;
    goto set;
three:
    angle = 3072;
set:
    p[1].rot.vy = angle;
    p[0].rot.vy = angle;
    goto done;
zero:
    p[1].rot.vy = 0;
zero_second:
    p[0].rot.vy = 0;
done:
    ;
}
