# Syscall ABI

Nova8 doesn't include built-in instructions for I/O, program exit, or other OS-level features. Instead, it uses a separate syscall mechanism — similar to real architectures.

`SYSCALL` is a special instruction that transfers control to the runtime kernel. Its behavior depends on the value in register `R1` (the syscall number). Arguments and return values are passed through registers.

### Register convention:

| Register | Purpose                      |
|----------|------------------------------|
| `R1`     | Syscall number               |
| `R0`     | First argument and/or return |

### Syscall Table (v0.1):

| Code   | Name    | Args | Return | Description                      |
|--------|---------|------|--------|----------------------------------|
| `0x01` | `print` | `R0` | —      | Prints the value in `R0`         |
| `0x02` | `read`  | —    | `R0`   | Reads a number from stdin        |
| `0x03` | `exit`  | `R0` | —      | Exits the program with code `R0` |

### Example

```asm
; print number
MOVI R0, 42
MOVI R1, 0x01
SYSCALL

; read number into R0
MOVI R1, 0x02
SYSCALL

; exit with code 0
MOVI R0, 0
MOVI R1, 0x03
SYSCALL
