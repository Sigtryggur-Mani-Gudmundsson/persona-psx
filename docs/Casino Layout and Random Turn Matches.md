# Layout codes and random preview turns

Two JP1 casino functions now compile to the original instructions and pass
all 11 complete binary SHA-256 checks in both the local and public snapshots.

| Function | Address | Bytes |
| --- | --- | ---: |
| `func_8008C5DC` | `0x8008C5DC` | 264 |
| `func_80093B7C` | `0x80093B7C` | 144 |

Total: 408 newly matched bytes. Internal branch and jump destinations were
also checked before integration.

The layout helper clears twenty texture indices and decodes words until the
next word's low byte is 10. A shared word index advances both the destination
and the input cursor; this retains the original input loads at offset two
and the saved-register initialization order. Unknown codes still trigger
the layout update before advancing to the next code.

The random helper writes the same quarter-turn to the current and previous
preview transforms. The two callers supply adjacent `CasinoXform` records;
their `rot.vy` members explain the original offsets 2 and 42. It calls `rand`
once, computes the signed remainder modulo four, and uses zero for negative
remainders. Identical negative-path arms are retained because folding them
changes cc1's register lifetime and one shift instruction's source register.
No assembly, forced registers, volatile barriers, or artificial padding are
used in either implementation.

## Native checks

`tests/native/casino_layout_codes.c` covers every 16-bit control word plus
mixed streams of lengths zero through twenty: 65,557 cases. It checks the
selected handler, destination, code argument, cleared slots, and layout
updates.

`tests/native/casino_random_turn.c` covers the complete PSYQ random range,
negative counterparts, and integer boundaries: 65,546 cases. It verifies
both rotations, exactly one random call, and every untouched byte.

Both fixtures pass optimized LTO and AddressSanitizer/UndefinedBehaviorSanitizer
builds. For example, from the repository root:

```sh
gcc -std=gnu89 -O2 -fno-builtin -flto -Werror=lto-type-mismatch \
  -Iinclude -Iinclude/psyq tests/native/casino_layout_codes.c \
  src/casino/game/layoutcodes.c -o /tmp/persona-layout-test
/tmp/persona-layout-test
```

For the random test, substitute `casino_random_turn.c` and `randomturn.c`.
These native fixtures are isolated behavior checks, not a complete Linux port.
