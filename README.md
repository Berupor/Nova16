# nova16 — a minimalist 16-bit virtual CPU system

Virtual computing architecture with registers, heap, stack, syscalls and assembler.

## Features

- 4 general-purpose 8-bit registers (`R0`–`R3`)
- 64KB linear memory space (byte-addressable)
- 256-byte stack with `PUSH`/`POP`/`CALL`/`RET`
- Custom bytecode with 16-bit addressing (`MOV`, `ADD`, `JMP`, `LOAD`, `STORE`, ...)
- Human-readable assembler (`.nova → .bin`)
- Kernel with `SYSCALL` support (`print`, `read`, `exit`, …)
- Modular segments: `TEXT`, `DATA`, `HEAP`, `STACK`

## Project structure

```
assembler/     # Go-based assembler producing .bin files
vm/            # C virtual machine implementation
programs/      # example Nova16 programs and libraries
lib/           # reusable libs (heap, math, etc)
documentation/ # ISA, syscalls, binary format
tests/         # automated test suite

Taskfile.yml   # build & run automation
```

## Example program

```asm
ENTRY: main

main: 
    MOVI R0, 5
    MOVI R1, 7
    ADD R0, R1
    MOVI R1, 0x01
    SYSCALL       ; syscall print(R0)
    HLT
```

## Why

To understand and build a complete low-level system:
- registers, stack, memory layout
- instruction decoding and control flow
- binary formats and program loading
- heap allocation
- kernel syscall ABI
- and how it all connects into a real working system

## 📚 Documentation

- [Instruction Set](documentation/opcode_table.md)
- [Syscall ABI](documentation/syscall_abi.md)
- [Binary Format](documentation/binary_format.md)
- [Register Convention](documentation/registers.md)
