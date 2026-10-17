/* Persona 1 (JP) - expire an idle state after 600 ticks. */
#include <decomp/types.h>

extern u_char D_800AAE9C;
extern u_char idle_state __asm__("D_800AAE94 + 0x2");
extern short D_800AAE98;
extern u_char idle_extra __asm__("D_800AAE98 + 0x2");

void func_8009065C(void)
{
    if (!D_800AAE9C && idle_state == 1) {
        if (++D_800AAE98 >= 600) {
            D_800AAE98 = 0;
            idle_state = 0;
            idle_extra = 0;
        }
    }
}
