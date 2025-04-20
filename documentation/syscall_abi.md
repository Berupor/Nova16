# Syscall ABI

Nova8 doesn't include built-in instructions for I/O, program exit, or other OS-level features.  
Instead, it uses a separate syscall mechanism — similar to real architectures.

`SYSCALL` is a special instruction that transfers control to the runtime kernel.  
Its behavior depends on the value in register `B` (the syscall number).  
Arguments and return values are passed through registers.

### Register convention:

| Register | Purpose                      |
|----------|------------------------------|
| `B`      | Syscall number               |
| `A`      | First argument and/or return |

### Syscall Table (v0.1):

| Code | Name    | Args | Return | Description                     |
|------|---------|------|--------|---------------------------------|
| `01` | `print` | `A`  | —      | Prints the value in `A`         |
| `02` | `read`  | —    | `A`    | Reads a number from stdin       |
| `03` | `exit`  | `A`  | —      | Exits the program with code `A` |

### Example

```asm
; print number
MOVI A, 42
MOVI B, 0x01
SYSCALL

; read number into A
MOVI B, 0x02
SYSCALL

; exit with code 0
MOVI A, 0
MOVI B, 0x03
SYSCALL
