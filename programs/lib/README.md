# Nova16 Library

This folder contains reusable low-level subroutines written in Nova16 assembly (`.nova`).

These are small, self-contained routines that can be *included* and *called* from any program, similar to how standard libraries work in real operating systems.

> Think of this like the `.text` section in a real binary that contains helper functions such as `memcpy`, `strlen`, or `malloc`.  
> Just like `libc` provides common utilities in Linux, this `lib/` folder is your standard library for Nova16.

## Structure

Each file:
- Starts with a **label** (which acts like a function name)
- Ends with a `RET` instruction
- Should avoid hardcoded memory addresses or global side-effects

Compiled `.bin` versions are placed into `bin/lib/*.bin` and can be directly `INCLUDE`'d.

## How to Use

In `.nova` file:

```asm
ENTRY: main
INCLUDE "lib/add_3.nova"

main:
    MOVI R0, 5
    CALL add_3
    ; R0 now contains 8
```