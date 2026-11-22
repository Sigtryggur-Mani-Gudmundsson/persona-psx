/* Persona 1 (JP) - draw the animated lines behind the map foreground. */
#include <decomp/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <persona/s2d/s2d.h>

extern int map_draw_frame __asm__("D_800A566C+0x10");
extern DR_MODE wave_modes[2] __asm__("D_800B5F5C");
extern LINE_G2 wave_lines[2][240] __asm__("D_800B5F5C+0x48");
extern void func_80096DE4(u_long *dst, u_long *src, u_long count);

void func_80096E0C(GsOT *ot)
{
    LINE_G2 *line = (LINE_G2 *)0x1F800000;
    int y;
    int shift;
    int left, right;
    int r, g, b;
    int frequency;

    SetLineG2(line);
    SetSemiTrans(line, 1);
    SetShadeTex(line, 0);
    y = 0;
    setRGB0(line, 128, 128, 128);
    setRGB1(line, 128, 128, 128);
    do {
        shift = (rsin(map_draw_frame * 105 + y * 87) * 64) >> 12;
        line->x0 = shift - 320;
        line->y0 = y - 120;
        line->x1 = line->x0 + 640;
        line->y1 = line->y0;

        left = (((rsin(map_draw_frame * 16 + (y << 6)) + 4096) / 2) * 88) >> 12;
        shift = left + 16;
        r = (((rsin(map_draw_frame * 5) + 4096) / 2) * 16) >> 12;
        g = (((rsin(map_draw_frame * 17) + 4096) / 2) * 6) >> 12;
        b = (((rsin(map_draw_frame * 37) + 4096) / 2) * 18) >> 12;
        setRGB0(line, left + 27 + r, left + 24 + g, shift + b);

        frequency = 56;
        right = (((rsin(map_draw_frame * frequency + (y << 5)) + 4096) / 2) * 88) >> 12;
        shift = right + 16;
        r = 16 - r;
        g = 6 - g;
        b = 18 - b;
        setRGB1(line, right + 27 + r, right + 24 + g, shift + b);

        func_80096DE4((u_long *)&wave_lines[g_draw_side][y], (u_long *)line, 5);
        addPrim(ot->org, &wave_lines[g_draw_side][y]);
        y++;
    } while (y < 240);

    SetDrawMode(&wave_modes[g_draw_side], 1, 1, 64, 0);
    AddPrim(ot->org, &wave_modes[g_draw_side]);
}
