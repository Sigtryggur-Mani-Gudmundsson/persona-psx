/* s2d's object for the shared unit in src/common.
 *
 * Each overlay compiles its own: the work area differs (WORK_BIAS) and splat
 * resolves symlinks when it writes the linker script, so a link here would
 * collapse back to one shared object.
 */
/* This overlay names the saved north-up option by its address. */
#define g_map_north_up D_801F2ACB
#include "../../../common/ui/configlist.c"
