# Casino dealer dispatcher

`func_8007D3CC` dispatches five byte-state values to their existing handlers.
All other values return without calling a handler. Preserving the case order
0x12, 0x35, 0x14, 0x70, 0x71 reproduces the original compiler's 176-byte branch
tree without introducing a jump table.

Acceptance requires 100% objdiff and all eleven JP1 linked binary SHA-256
checks in the pinned original-compiler environment.

The 0x71 handler takes `play_step` and writes 0x25 to that byte. Its declaration
and call must both include the pointer. The previous no-argument declaration
happened to preserve the incoming argument register in the matching PS1 build,
but disagreed with the callee's C signature. Passing the pointer explicitly
preserves all eleven original binaries and makes the C interface valid for
native compilation; this correction does not add newly matched bytes.

`tests/native/dealer_dispatch.c` checks all 256 states, all 256 initial byte
values and three pointer destinations: 196,608 cases. It verifies the selected
byte, neighboring bytes, handler dispatch and unchanged dispatcher state.
Compile the actual dispatcher and card-reset handler with:

```sh
cc -std=gnu89 -O2 -fno-builtin -flto -Werror=lto-type-mismatch \
  -Iinclude -Iinclude/psyq tests/native/dealer_dispatch.c \
  src/casino/game/dealerdispatch.c src/casino/game/cardreset.c \
  -o /tmp/persona-dealer-dispatch
/tmp/persona-dealer-dispatch
```

The old declaration fails this link-time type check. The corrected source also
passes the fixture with `-O1 -g -fsanitize=address,undefined
-fno-sanitize-recover=all` instead of the optimization/LTO flags above, using
`ASAN_OPTIONS=detect_leaks=0`. This is a focused host fixture, not a native game
startup test or emulator playtest.
