# Map debug menu input

`func_8008BD78` handles map debug-menu actions, field selectors, fade toggling, and menu navigation. The fade flag uses a scalar view of the original field at `g_fade + 8` to preserve its absolute accesses. Structured switch cases reproduce the compiler's shared tails.

The function is 704 bytes with a 48-byte jump table at `0x80064F08`. Isolated normalized and linked code match. The final full build passes all eleven JP1 SHA-256 checks; objdiff reports 100% for the function.
