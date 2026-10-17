/* Persona 1 (JP) - reset three fixed-point scale components and state. */
#include <decomp/types.h>

extern long D_800B5F2C, D_800B5F30, D_800B5F34;
extern char D_800AA73C[];

void func_8009C33C(void)
{
    D_800B5F2C = 0x1000;
    D_800B5F30 = 0x1000;
    D_800B5F34 = 0x1000;
    *(long *)(D_800AA73C + 0x5C1C) = 3;
}
