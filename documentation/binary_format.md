# Binary format

Each compiled `.bin` file starts with an 8-byte header, followed by the actual bytecode and data.

### Header Layout (8 bytes)

| Offset | Size    | Name       | Description                                              |
|--------|---------|------------|----------------------------------------------------------|
| `0x00` | 1 byte  | `MAGIC_1`  | Magic byte #1: `0xDE`                                    |
| `0x01` | 1 byte  | `MAGIC_2`  | Magic byte #2: `0xAD`                                    |
| `0x02` | 1 byte  | `VERSION`  | Binary format version (currently `0x01`)                 |
| `0x03` | 1 byte  | `ENTRY`    | Entry point offset (sets the initial `PC` of the CPU)    |
| `0x04` | 1 byte  | `CODESIZE` | Size of the code section (after the header)              |
| `0x05` | 3 bytes | `RESERVED` | Reserved for future use (e.g. flags, memory hints, etc.) |

Total size: `8 + CODESIZE` bytes.

### Example

Given this input:

```asm
ENTRY: main
INCLUDE "lib/math.v8asm"

main:
    MOVI A, 5
    CALL add_3
    HLT
```

It compiles into:

```
[DE AD 01 10 2A 00 00 00]  # Header
[... actual bytecode ...] # Bytecode starts at offset 0x08
```
