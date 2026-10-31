/* Persona 1 (JP) - queue a draw-mode change for each texture page in use. */
#include <persona/s2d/s2d.h>

/* The page slots func_80097DBC fills (DrawModePages.pages elsewhere) and,
   after them, one row of draw modes per display buffer. Reached here as two
   arrays: as one structure, cse shares their base and the image does not. */
extern short   D_800B56F8[16];
extern DR_MODE D_800B5718[2][16];

/* For every page slot in use, sets the slot's draw mode to that texture
   page and links it into ordering-table entry `i`. */
void func_80097CFC(GsOT *ot, int buf)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (D_800B56F8[i] != -1) {
            SetDrawMode(&D_800B5718[buf][i], 0, 0, D_800B56F8[i], 0);
            AddPrim(ot->org + i, &D_800B5718[buf][i]);
        }
    }
}
