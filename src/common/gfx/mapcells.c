/* Persona 1 (JP) - the cell definitions behind the character maps.
 *
 * Compiled into three overlays rather than called across the boundary:
 *                  DNG         ADV         S2D
 *   FontCellsInit  0x8008B36C  0x8007CDD4  0x8007B7D4
 *
 * A GsMAP draws through two arrays: an index array saying which cell sits in
 * each map square, and a cell array saying what each cell shows. This fills
 * the cell array the text map points at - the screen setup at 0x80068A7C
 * stores g_font_cells straight into that map's `base`.
 *
 * The glyphs are an 8x12 atlas 31 columns wide starting at v 0x50, so cell
 * n + 1 is glyph n and cell 0 is the blank at the far corner of the page. That
 * offset by one is why an index array cleared to zero draws as empty space.
 *
 * S2D builds this same source against a work area 0x20000 higher, which is
 * what WORK_BIAS says.
 */
#include <decomp/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

/* The blank sits one cell before the atlas. */
#define g_font_blank ((GsCELL *)(0x800E864C + WORK_BIAS))
#define g_font_cells ((GsCELL *)(0x800E8654 + WORK_BIAS))

/* JP1 ADV and DNG address the blank cell fields by their linker symbols. */
#if defined(VER_JP1) && WORK_BIAS == 0
extern u_char font_blank_u __asm__("D_800E864C");
extern u_char font_blank_v __asm__("D_800E864D");
extern u_short font_blank_cba __asm__("D_800E864E");
extern u_short font_blank_flag __asm__("D_800E8650");
extern u_short font_blank_tpage __asm__("D_800E8652");
#else
#define font_blank_u g_font_blank->u
#define font_blank_v g_font_blank->v
#define font_blank_cba g_font_blank->cba
#define font_blank_flag g_font_blank->flag
#define font_blank_tpage g_font_blank->tpage
#endif

#define GLYPH_W    8
#define GLYPH_H    12
#define GLYPH_COLS 31
#define GLYPH_ROWS 4
#define FONT_V     0x50
#define FONT_CLUT  0x7E80
#define FONT_TPAGE 0x18

void FontCellsInit(void)
{
    short i;

    font_blank_u = 0xF8;
    font_blank_v = 0xF4;
    font_blank_cba = FONT_CLUT;
    font_blank_flag = 0;
    font_blank_tpage = FONT_TPAGE;
    for (i = 0; i < GLYPH_COLS * GLYPH_ROWS; i++) {
        g_font_cells[i].u = (i % GLYPH_COLS) * GLYPH_W;
        g_font_cells[i].v = (i / GLYPH_COLS) * GLYPH_H + FONT_V;
        g_font_cells[i].cba = FONT_CLUT;
        g_font_cells[i].flag = 0;
        g_font_cells[i].tpage = FONT_TPAGE;
    }
}
