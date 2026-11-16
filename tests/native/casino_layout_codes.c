#include <persona/casino/casino.h>

extern int printf(const char *, ...);
extern void func_8008C5DC(CasinoLayout *, u_short *);
static CasinoLayout layout;
static u_short textures[20], stream[22];
static unsigned flushes, decodes, failed;

static void decode(u_short *out, u_short code, unsigned kind)
{
    if (flushes >= 20 || out != textures + flushes
        || code != stream[flushes + 1] || (code & 255) != kind)
        failed = 1;
    *out = code;
    decodes++;
}
void func_8008C6E4(u_short *p, u_short c) { decode(p, c, 0x81); }
void func_8008C724(u_short *p, u_short c) { decode(p, c, 0x82); }
void func_8008C81C(u_short *p, u_short c) { decode(p, c, 0x83); }
void CasinoTexLayout(CasinoLayout *p)
{
    if (p != &layout) failed = 1;
    flushes++;
}

static int check(unsigned length)
{
    unsigned i, expected = 0;
    for (i = 0; i < 20; i++) textures[i] = 0xFFFF;
    layout.tex_idx = textures;
    flushes = decodes = failed = 0;
    func_8008C5DC(&layout, stream);
    if (flushes != length || failed) return 1;
    for (i = 0; i < 20; i++) {
        unsigned kind = stream[i + 1] & 255;
        int recognized = i < length && kind >= 0x81 && kind <= 0x83;
        expected += recognized;
        if (textures[i] != (recognized ? stream[i + 1] : 0)) return 1;
    }
    return decodes != expected;
}

int main(void)
{
    unsigned code, length, i, cases = 0;
    for (code = 0; code < 65536; code++) {
        stream[1] = code;
        stream[2] = 10;
        if (check((code & 255) == 10 ? 0 : 1)) return 1;
        cases++;
    }
    for (length = 0; length <= 20; length++) {
        for (i = 0; i < length; i++)
            stream[i + 1] = (i << 8) | (0x80 + i % 5);
        stream[length + 1] = 10;
        if (check(length)) return 1;
        cases++;
    }
    printf("casino layout codes: %u cases passed\n", cases);
    return 0;
}
