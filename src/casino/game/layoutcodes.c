/* Decode the layout's two-byte control codes, ending at a low byte of 10. */
#include <persona/casino/casino.h>

extern void func_8008C6E4(u_short *, u_short);
extern void func_8008C724(u_short *, u_short);
extern void func_8008C81C(u_short *, u_short);
extern void CasinoTexLayout(CasinoLayout *);

void func_8008C5DC(CasinoLayout *layout, u_short *codes)
{
    int i;
    int index;
    u_short *current;

    for (i = 0; i < 20; i++)
        layout->tex_idx[i] = 0;
    current = codes;
    if ((u_char)codes[1] != 10) {
        index = 0;
        do {
            switch ((u_char)current[1]) {
            case 0x81:
                func_8008C6E4(layout->tex_idx + index, current[1]);
                break;
            case 0x82:
                func_8008C724(layout->tex_idx + index, current[1]);
                break;
            case 0x83:
                func_8008C81C(layout->tex_idx + index, current[1]);
                break;
            }
            index++;
            current = codes + index;
            CasinoTexLayout(layout);
        } while ((u_char)current[1] != 10);
    }
}
