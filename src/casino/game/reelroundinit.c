/* Persona 1 (JP) - initialize the reel game state and frame layouts. */
#include <persona/casino/casino.h>

typedef struct {
    u_char *row[21];
} ReelSymbolTable;

extern ReelSymbolTable D_800B3E28;
extern ReelSymbolTable *D_8009E940[][2];
extern u_char *D_8009E93C[][2];
extern int D_800A8EA4, g_casino_money_shown;
extern u_char D_800B0AF8[], D_800AF73C[], D_800AF9E0[], D_800AF830[];
extern u_char reel_state[] __asm__("D_800AF834");
extern signed char D_800AF9D0[];
extern CasinoLayout *D_800A1AA0[], *D_800A1AA8[];
extern CasinoLayout *D_800A20E8[], *D_800A20F0[];

void func_800853F4(void)
{
    int i;
    u_char *ticks, *frames, *delta, *base;

    D_800A8EA4 = 1;
    g_casino_bet_shown = 255;
    D_800B0AF8[1] = 0;
    D_800AF73C[2] = 0;
    D_800AF73C[3] = 0;
    D_800AF9E0[3] = 0;
    D_800AF9E0[2] = 0;
    D_800AF9E0[1] = 0;
    D_800AF9E0[0] = 0;
    frames = reel_state;
    frames[6] = 0;
    frames[5] = 0;
    D_800AF830[0] = 3;
    frames[0] = 3;
    D_800AF830[3] = 3;
    D_800AF830[2] = 3;
    D_800AF830[1] = 3;
    g_casino_bet_done = 0;
    g_casino_quit = 0;
    g_casino_win = 0;
    g_casino_bet = 0;
    (*(CasinoFlags *)&g_casino_lamps).b4 = 0;
    (*(CasinoFlags *)&g_casino_lamps).b0 = 0;
    (*(CasinoFlags *)&g_casino_lamps).b2 = 0;
    g_casino_money_shown = g_casino_money - 1;
    D_800B3E28 = *D_8009E940[g_casino_spot - 216][0];

    {
        CasinoLayout **first, **second;
        int slot, offset, status;
        u_char symbol, initial_speed;

        i = 0;
        initial_speed = 8;
        base = reel_state + 0xDA;
        delta = base + 0x3F;
        frames = base + 0x2A;
        second = (CasinoLayout **)(base - 0x7E);
        offset = -72;
        first = (CasinoLayout **)(base - 0xD2);
        slot = -5;
        ticks = base;
        for (; i < 21; delta++, frames++, second++, offset += 16,
                        first++, slot++, i++, ticks += 2) {
            reel_state[0xB5 + slot] = 0;
            reel_state[0xCA + slot] = 0;
            *(short *)ticks = 0;
            reel_state[0x148 + slot] = 0;
            symbol = D_8009E93C[g_casino_spot - 216][0][i];
            status = -1;
            *(signed char *)((u_long)D_800AF9D0 + slot) = status;
            status = i < 5;
            reel_state[0x187 + slot] = 0;
            reel_state[0x133 + slot] = 0;
            reel_state[0x15D + slot] = symbol;

            /* The two frame banks use the same offset from their anchors. */
            if (status) {
                *first = *(CasinoLayout **)((u_long)D_800A1AA0 + offset);
                *second = *(CasinoLayout **)((u_long)D_800A1AA8 + offset);
            } else {
                *first = *(CasinoLayout **)((u_long)D_800A20E8 + offset);
                *second = *(CasinoLayout **)((u_long)D_800A20F0 + offset);
            }
            *frames = initial_speed;
            *delta = initial_speed;
        }
    }

    i = 3;
    delta = D_800AF9E0;
    for (; i >= 0; i--) {
        delta[i] = 0;
    }
}
