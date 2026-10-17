#include <decomp/types.h>

typedef struct
{
    u8 pad00[0x78];
    double value78;
    u8 pad80[0x18];
    double value98;
    u8 padA0[0x20];
    double valueC0;
    u8 padC8[0x18];
    s8 mode;
} CasinoBounce;
void func_8009190C(CasinoBounce *value)
{
    value->valueC0 = -0.2 * value->valueC0;
    value->value78 = 7.7 + value->value78;
    if (value->mode == 0)
    {
        value->value98 = 100.0;
    }
}
