# Register Reference

| Name | Code | Description                                                    |
|------|------|----------------------------------------------------------------|
| `R0` | 0x00 | General-purpose. Used for return values, math operations, etc. |
| `R1` | 0x01 | General-purpose. Often used for syscall IDs, arguments         |
| `R2` | 0x02 | Temporary register, scratch space                              |
| `R3` | 0x03 | General-purpose or secondary return, caller-saved              |

All registers are **general-purpose** and can be used interchangeably. However, certain conventions are suggested.

## Soft ABI

| Usage          | Register   |
|----------------|------------|
| Return value   | `R0`       |
| Argument #1    | `R1`       |
| Argument #2    | `R2`       |
| Temporary      | `R2`, `R3` |
| Syscall number | `R1`       |
| Syscall return | `R0`       |

Example syscall:

```asm
MOVI R1, 0x01    ; syscall: print
MOVI R0, 42      ; argument: value to print
SYSCALL
```
