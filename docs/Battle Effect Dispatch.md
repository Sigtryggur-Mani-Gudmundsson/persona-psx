# Battle effect argument preservation

`BtlTickEffects` at JP1 `0x8008AF80` forwards its object to one of two handlers.
The original `jal BtlTalkFxObjTick` at `0x8008AF9C` leaves the incoming object
pointer in `$a0`. The C call must pass `o` explicitly; relying on a register
left over from the caller is undefined behavior in C and is unsafe for a
native port. The shared object header now declares both effect handlers and
the dispatcher with their pointer arguments.

The corrected call retains all 11 complete JP1 binary SHA-256 matches. This
is a correction to already matching C, so it adds no new matched bytes.

## Native regression check

The test links the actual dispatcher and negotiation-effect implementation.
It exercises 36,864 combinations of effect kinds, phases, timers, dispatch
and animation flags, wave directions, and object addresses. Only the ordinary
effect handler and object-free operation are replaced by pointer-recording
stubs. This checks argument forwarding, negotiation transitions, drift, and
which object is freed; it is not a full native game or PS1 layout test.

Run from the repository root, choosing an output path outside tracked files:

```sh
gcc -std=gnu89 -O2 -fno-builtin -flto -Werror=lto-type-mismatch \
  -Iinclude -Iinclude/psyq tests/native/battle_effect_dispatch.c \
  src/btlp/grouptick.c src/btlp/fxtalktick.c -o /tmp/persona-effect-dispatch
/tmp/persona-effect-dispatch
```

The same cases pass with AddressSanitizer and UndefinedBehaviorSanitizer
(`-O1 -g -fsanitize=address,undefined -fno-sanitize-recover=all` in place of
the optimization/LTO flags). As a negative control, the previous dispatcher
compiled against the corrected shared header is rejected for calling
`BtlTalkFxObjTick` with too few arguments. The old unprototyped declaration
alone did not make LTO reject the call; the shared prototype is essential.

## Motion table calls

`BtlTickPersona`, `BtlTickEnemy`, and `BtlTickMember` likewise pass the object
explicitly through typed motion-table pointers. Their original indirect calls
at `0x8008AFF8`, `0x8008B100`, and `0x8008B164` preserve the incoming `$a0`.
All 22 distinct non-null handlers in the three original tables take `BtlObj *`.
The correction preserves null-entry behavior, held fighters, and the Persona
position and colour updates after its handler returns.

`tests/native/battle_motion_dispatch.c` links the actual three dispatchers
with pointer-recording table entries. Its 36 cases cover all three groups,
two distinct object addresses, null and two populated entries, and held or
unheld records. Run it with the same compiler flags above, linking
`src/btlp/grouptick.c` only. Both optimized LTO and sanitizer builds pass.

## Spell step calls

The indirect step call in `BtlFxObjTick` at `0x800B6C04` also retains `$a0`.
All 27 distinct step handlers in `g_btl_spell_fx` take an object pointer.
The table member, local function pointer, and shared step declarations now
express that signature, and the dispatcher calls `step(o)`. Tracking objects
still bypass the step handler, follow camera rotation, and rearm their script
only when animation has stopped. All 11 JP1 hashes remain unchanged.

`tests/native/battle_spell_dispatch.c`, linked with `src/btlp/fxtick.c`, checks
432 combinations of callback entries, motion states, tracking/animation
flags, object addresses, camera rotations, and script pointers. Optimized
LTO and AddressSanitizer/UndefinedBehaviorSanitizer builds pass. Removing the
explicit argument while retaining the typed pointer fails compilation.
