#include <persona/casino/blackjack.h>

extern int printf(const char *, ...);
extern void func_8007D3CC(unsigned char *play_step);

unsigned char D_800A8C7C;
static unsigned calls[4];

void func_8007D47C(void) { calls[0]++; }
void func_8007D4B8(void) { calls[1]++; }
void func_8007D554(void) { calls[2]++; }
void func_8007D714(void) { calls[3]++; }

int main(void)
{
    unsigned state, value, selected, i, cases = 0;
    static const unsigned states[4] = {0x12, 0x35, 0x14, 0x70};
    unsigned char bytes[3], expected[3];

    for (state = 0; state < 256; state++) {
        for (value = 0; value < 256; value++) {
            for (selected = 0; selected < 3; selected++) {
                for (i = 0; i < 3; i++) {
                    bytes[i] = expected[i] = (unsigned char)(value + i);
                }
                for (i = 0; i < 4; i++) calls[i] = 0;
                D_800A8C7C = (unsigned char)state;
                if (state == 0x71) expected[selected] = 0x25;

                func_8007D3CC(&bytes[selected]);

                for (i = 0; i < 3; i++) {
                    if (bytes[i] != expected[i]) {
                        printf("Wrong byte: state=%u value=%u selected=%u index=%u\n",
                               state, value, selected, i);
                        return 1;
                    }
                }
                for (i = 0; i < 4; i++) {
                    if (calls[i] != (state == states[i])) return 1;
                }
                if (D_800A8C7C != state) return 1;
                cases++;
            }
        }
    }
    printf("dealer dispatcher: %u cases passed\n", cases);
    return 0;
}
