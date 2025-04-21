# nova8 — a virtual 8-bit processor

Started as a simple CPU and grew into a minimal virtual computing system.

## Features

- 2 registers (`A`, `B`)
- 256 bytes of memory
- 16-byte stack
- basic heap with `MALLOC` / `FREE`
- custom bytecode: `MOV`, `ADD`, `JMP`, `CALL`, `RET`, `CMP`, `SYSCALL`
- Assembler (`.v8asm → .bin`)
- kernel subsystem for system calls (`print`, `read`, `exit`)

## Project structure

```
assembler/     # the assembler (Python)
src/           # virtual machine (CPU, memory, stack, kernel)
include/       # headers
programs/      # .v8asm test programs
Taskfile.yml   # build & run automation
```

## Quick start

```bash
task run
```

Compiles `.v8asm` → `.bin`, builds the VM and runs the program.

## Example program (.v8asm)

```asm
MOVI A, 5
MOVI B, 7
ADD A, B
MOVI B, 0x01
SYSCALL       ; print A
HLT
```

## Why

To understand how things work under the hood — registers, stack, bytecode, memory — and how it all comes together into a working architecture.

## Documentation

- [Instruction Set / Opcodes](documentation/opcode_table.md)
- [Syscall ABI](documentation/syscall_abi.md)
- [Binary format](documentation/binary_format.md)
