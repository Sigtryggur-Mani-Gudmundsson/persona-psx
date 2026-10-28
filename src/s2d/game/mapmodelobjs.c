/* Persona 1 (JP) - map a TMD and point the map's own object slots at it. */
#include <persona/s2d/s2d.h>
#include <decomp/include_asm.h>

extern u_long g_model_objs[256];

/* S2dMapModels for the ids listed in the map's g_map_info record, after its
   effect and up to an 0xFF: maps the TMD in place, points each slot at its
   object and resolves each object's primitive pointer. As there, `tmd`
   walks the header in one register.

   NON_MATCHING: the structure matches (indexing g_map_info[map] inside the
   loop is what keeps `map` live in s1, as the image has it), but cc1 gives
   tmd s1 and map s0 the other way round, and points the id walk at the
   record rather than at pad[0]. */
#ifdef NON_MATCHING
void func_80097BE8(int tmd, short map)
{
    int i;

    tmd += 4;
    GsMapModelingData((u_long *)tmd);
    tmd += 8;
    for (i = 0; g_map_info[map].pad[i] != 0xFF; i++) {
        g_model_objs[g_map_info[map].pad[i]] = tmd;
        g_model_objs[g_map_info[map].pad[i]] &= 0x7FFFFFFF;
        *(u_long *)((u_long *)tmd)[4] &= 0xFF000000;
        *(u_long *)((u_long *)tmd)[4] |= ((u_long *)tmd)[5];
        tmd += 28;
    }
}
#else
INCLUDE_ASM("s2d/nonmatchings/game/mapmodelobjs", func_80097BE8);
#endif
