#include <persona/casino/casino.h>
#include <rand.h>

extern int printf(const char *, ...);
extern void func_80093B7C(CasinoXform *pair);

static int supplied, calls;

int rand(void)
{
    calls++;
    return supplied;
}

static int check(int value)
{
    struct {
        unsigned char before[16];
        CasinoXform pair[2];
        unsigned char after[16];
    } state;
    unsigned char *bytes = (unsigned char *)&state;
    unsigned long first, second, i;
    int remainder, angle;

    for (i = 0; i < sizeof state; i++) bytes[i] = 0xA5;
    first = (unsigned char *)&state.pair[0].rot.vy - bytes;
    second = (unsigned char *)&state.pair[1].rot.vy - bytes;
    supplied = value;
    calls = 0;
    func_80093B7C(state.pair);

    /* A negative remainder follows the original default/zero-angle path. */
    remainder = value % 4;
    angle = remainder > 0 ? remainder * 1024 : 0;
    if (calls != 1 || state.pair[0].rot.vy != angle
        || state.pair[1].rot.vy != angle) return 1;

    /* Check every other field and the guards, using native structure sizes
       rather than assuming PS1 pointer/long sizes on the host. */
    for (i = 0; i < sizeof state; i++) {
        if (i >= first && i < first + sizeof state.pair[0].rot.vy) continue;
        if (i >= second && i < second + sizeof state.pair[1].rot.vy) continue;
        if (bytes[i] != 0xA5) return 2;
    }
    return 0;
}

int main(void)
{
    static const int boundaries[] = {
        (-2147483647 - 1), -2147483647, -2147483646, -2147483645,
        -2147483644, 2147483643, 2147483644, 2147483645,
        2147483646, 2147483647
    };
    int value;
    unsigned i, cases = 0;

    for (value = -32768; value <= RAND_MAX; value++) {
        if (check(value)) {
            printf("random turn failed: value=%d\n", value);
            return 1;
        }
        cases++;
    }
    for (i = 0; i < sizeof boundaries / sizeof boundaries[0]; i++) {
        if (check(boundaries[i])) {
            printf("random turn failed: boundary=%d\n", boundaries[i]);
            return 1;
        }
        cases++;
    }
    printf("casino random turn: %u cases passed\n", cases);
    return 0;
}
