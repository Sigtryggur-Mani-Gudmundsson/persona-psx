/* Persona 1 (JP) - one font glyph into VRAM.
 *   DNG 0x800770EC   ADV 0x80067924   S2D 0x80067114
 *
 * Expand the 1bpp glyph into a 4bpp staging cell, then queue a 16x16
 * upload. Colour 1 draws the shadow a row down and two pixels right.
 * Colour 2 draws the glyph on top.
 */
#include <decomp/types.h>
#include <persona/common/font.h>
#include <persona/common/imageanim.h>

#define PIXELS(buf, row, pair) ((u_char *)((row) * 8 + (u_long)(buf)) + (pair) * 4)

static inline int ShadowPair0(int bits)
{
    u_int high = bits & 0x40;
    u_int low = bits;

    return (low >> 7) + (high >> 2);
}

static inline int ShadowPair1(int bits)
{
    u_int high = bits & 0x10;
    u_int low = bits & 0x20;

    return high + (low >> 5);
}

static inline int ShadowPair2(int bits)
{
    u_int high = bits & 0x04;
    u_int low = bits & 0x08;

    return (low >> 3) + (high << 2);
}

static inline int ShadowPair3(int bits)
{
    u_int high = bits & 0x01;
    u_int low = bits & 0x02;

    return (low >> 1) + (high << 4);
}

static inline int GlyphPair2(int bits)
{
    u_int high = bits & 0x04;
    u_int low = bits & 0x08;

    return (low >> 2) + (high << 3);
}

static inline int GlyphPair3(int bits)
{
    u_int high = bits & 0x01;
    u_int low = bits & 0x02;

    return low + (high << 5);
}

void FontUploadGlyph(short x, short y, u_short glyph)
{
    u_char (*src)[2];
    u_char (*buf)[8];
    u_char i;
    u_char j;
    u_int pixel, low, high;

    src = (u_char (*)[2])(g_font_bits + glyph * GLYPH_BYTES);
    buf = (u_char (*)[8])g_glyph_buf;
    for (i = 0; i < 8; i++) {
        buf[0][i] = 0;
    }

    /* The shift carries across row boundaries in the work area.
       The last shadow byte follows the 16 upload rows. */
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 2; j++) {
            PIXELS(buf, i, j)[9] = ShadowPair0(src[i][j]);
            PIXELS(buf, i, j)[10] = ShadowPair1(src[i][j]);
            PIXELS(buf, i, j)[11] = ShadowPair2(src[i][j]);
            PIXELS(buf, i, j)[12] = ShadowPair3(src[i][j]);
        }
    }

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 2; j++) {
            u_char *source = (u_char *)(i * 2 + (u_long)src) + j;
            u_int bits;

            /* The first two pairs reuse the same temporaries for the
               staged glyph bits and the existing shadow byte. */
            bits = *source;
            pixel = bits & 0x40;
            low = bits & 0x80;
            low >>= 6;
            high = pixel >> 1;
            pixel = PIXELS(buf, i, j)[0];
            low += high;
            pixel |= low;
            PIXELS(buf, i, j)[0] = pixel;

            bits = *source;
            high = bits & 0x10;
            bits &= 0x20;
            bits >>= 4;
            high <<= 1;
            pixel = PIXELS(buf, i, j)[1];
            bits += high;
            pixel |= bits;
            PIXELS(buf, i, j)[1] = pixel;

            bits = *source;
            PIXELS(buf, i, j)[2] = PIXELS(buf, i, j)[2] | GlyphPair2(bits);
            bits = *source;
            PIXELS(buf, i, j)[3] = PIXELS(buf, i, j)[3] | GlyphPair3(bits);
        }
    }

    g_image_queue[g_image_queue_count].data = (u_long *)g_glyph_buf;
    g_image_queue[g_image_queue_count].rect.x = x;
    g_image_queue[g_image_queue_count].rect.y = y;
    g_image_queue[g_image_queue_count].rect.w = 4;
    g_image_queue[g_image_queue_count].rect.h = 16;
    g_image_queue_count++;
}
