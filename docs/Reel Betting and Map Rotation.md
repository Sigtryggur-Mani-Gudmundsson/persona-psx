# Reel betting and map rotation

## Reel betting

`func_80082E1C` starts the reel bet palette animation, accepts coins on alternate
frames in maximum-bet mode, updates the bet display, and handles acceptance or
exit. The layout pointer is the `l` field of the existing `CasinoLayoutDef`.
Reading that field through its parent structure restores the original compiler's
argument setup; treating it as a standalone pointer changes scheduling.

The displayed-bet sentinel255 skips a zero-bet scroll. Display changes read the
current bet again after palette callbacks. The last coin does not play the
insertion sound when it simultaneously reaches eight and leaves no money.
Acceptance changes step112 and timer-1; exit refunds after sound callbacks,
clears the relevant lamps, fades the table, and selects game31.

Sixteen native fixtures cover input delay, alternating frames, the last coin,
positive and negative display shifts, the sentinel, exact-one flags, acceptance,
refunds, and callbacks changing the bet or balance. The palette mock addresses
the original PS1 on-byte offset; it does not interpret the host-sized palette
structure. No display or audio rendering is tested here.

## Map rotation

`func_8008BA9C` decrements a positive turn countdown, adds128 times the signed
turn step to the map's16-bit Y rotation, and updates input state. At the last
step it writes the saved facing and sets `g_key_lock` to1. Zero and negative
countdowns leave the state unchanged.

Direct access to `g_map_xform.rot.vy` matches the original120-byte routine.
Using a separate pointer temporary changes instruction order. The native
fixture covers120 boundary combinations, including signed rotation wrapping,
countdowns at zero and one, and preservation of other transform fields.

Both routines require all eleven original JP1 binary SHA-256 checks. Native
fixtures also run with address and undefined-behavior sanitizers; they do not
establish emulator gameplay or a native Linux game boot.

## Panel sliding

`func_80087F48` moves three layouts in alternating directions while translating
their associated object. Byte zero slides out; every other byte slides in.
Each subsequent layout is read after the preceding tween call.

All three layout pointers are read as `CasinoLayoutDef.l` fields. Horizontal
tween distance is a full-width `int`, narrowed when stored in the tween argument
record. The other tween parameters keep their existing widths; changing stack
parameter widths alters the original wrapper's load instructions. The wrapper
and existing callers retain their original bytes. The panel routine's512 native
fixtures cover all byte modes and callbacks replacing the next layout pointer.

## Reward-digit reveal

`func_800913FC` accepts a byte value. Values below ten update and animate the
single digit; other values write the remainder and quotient to the ones and
tens models, update those textures in that order, and animate tens then ones.
The three adjacent `CasinoModel` entries share one typed array. Object pointers
are read after texture callbacks, as in the original assembly.

The native fixture covers all 256 byte inputs both with and without callbacks
replacing the animated objects. It removes only the PS1 linker alias so the same
C body can use a host-sized model array. The 512 cases also check unused digits
and call order. Sanitizers and all eleven binary hashes are acceptance gates.
