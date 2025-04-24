# Binary Format

Each compiled `.bin` file starts with a **10-byte header**, followed by the actual bytecode and data.

### Header Layout

| Offset | Size    | Name       | Description                                                      |
|--------|---------|------------|------------------------------------------------------------------|
| `0x00` | 1 byte  | `MAGIC_1`  | Magic byte #1: `0xDE`                                            |
| `0x01` | 1 byte  | `MAGIC_2`  | Magic byte #2: `0xAD`                                            |
| `0x02` | 1 byte  | `VERSION`  | Format version (currently `0x02` for `nova16`)                   |
| `0x03` | 2 bytes | `ENTRY`    | Entry point (initial `PC` value), little-endian (lo, hi)         |
| `0x05` | 2 bytes | `CODESIZE` | Size of the code section (in bytes), little-endian (lo, hi)      |
| `0x07` | 3 bytes | `RESERVED` | Reserved for future use (e.g. heap base, flags, architecture ID) |