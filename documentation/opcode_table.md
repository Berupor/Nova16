# Instruction Set

Nova16 uses a 16-bit address space (64K), 8-bit registers, and a fixed-width instruction encoding. All opcodes are 1 byte. Memory addresses are 2 bytes, little-endian ([lo][hi]). Immediate values and registers remain 8-bit.

## v0.1 (Basic Instruction Set)

| Opcode | Name                   | Format                   | Description                                    |
|--------|------------------------|--------------------------|------------------------------------------------|
| `0x01` | `MOV dst_reg, src_reg` | `[OP][dst_dst][src_reg]` | Copies the value from one register to another  |
| `0x02` | `MOVI reg, val`        | `[OP][reg][imm]`         | Loads an immediate value into a register       |
| `0x10` | `PUSH reg`             | `[OP][reg]`              | Pushes the value of a register onto the stack  |
| `0x11` | `POP reg`              | `[OP][reg]`              | Pops a value from the stack into a register    |
| `0x20` | `ADD dst_reg, src_reg` | `[OP][dst_reg][src_reg]` | Adds two register values and stores in `dst`   |
| `0x21` | `SUB dst_reg, src_reg` | `[OP][dst_reg][src_reg]` | Subtracts `src` from `dst` and stores in `dst` |
| `0x30` | `CMP reg_a, reg_b`     | `[OP][reg_a][reg_b]`     | Sets `ZF` if values in `a` and `b` are equal   |
| `0x40` | `JMP addr`             | `[OP][addr]`             | Unconditional jump to address                  |
| `0x41` | `JZ addr`              | `[OP][addr]`             | Jump if `ZF == 1`                              |
| `0xF0` | `NOP`                  | `[OP]`                   | No operation                                   |
| `0xFF` | `HLT`                  | `[OP]`                   | Halts execution                                |

## v0.2 (Control Flow and Loops)

| Opcode | Name   | Format       | Description                               |
|--------|--------|--------------|-------------------------------------------|
| `0x22` | `INC`  | `[OP][reg]`  | Increments the value of a register by 1   |
| `0x23` | `DEC`  | `[OP][reg]`  | Decrements the value of a register by 1   |
| `0x50` | `CALL` | `[OP][addr]` | Pushes return address and jumps to `addr` |
| `0x51` | `RET`  | `[OP]`       | Pops return address and jumps back        |

## v0.3 (SYSCALL)

| Opcode | Name      | Format | Description                                                                  |
|--------|-----------|--------|------------------------------------------------------------------------------|
| `0x70` | `SYSCALL` | `[OP]` | Triggers a system call. Syscall number in `R1`, arguments and return in `R0` |

## v0.4 (Memory)

| Opcode | Name    | Format            | Description                       |
|--------|---------|-------------------|-----------------------------------|
| `0x60` | `LOAD`  | `[OP][reg][addr]` | Loads data from addr to register  |
| `0x61` | `STORE` | `[OP][addr][reg]` | Writes data from register to addr |

### v0.5 (Memory Access with 16-bit Addressing)

| Opcode | Name    | Format              | Description                            |
|--------|---------|---------------------|----------------------------------------|
| `0x40` | `JMP`   | `[OP][lo][hi]`      | Unconditional jump to 16-bit address   |
| `0x41` | `JZ`    | `[OP][lo][hi]`      | Jump to address if ZF == 1             |
| `0x50` | `CALL`  | `[OP][lo][hi]`      | Call subroutine, pushes return address |
| `0x51` | `RET`   | `[OP]`              | Return from subroutine                 |
| `0x60` | `LOAD`  | `[OP][reg][lo][hi]` | Load from memory into register         |
| `0x61` | `STORE` | `[OP][lo][hi][reg]` | Store value from register into memory  |
