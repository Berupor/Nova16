# Instruction Set

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

| Opcode | Name      | Format | Description                                                                |
|--------|-----------|--------|----------------------------------------------------------------------------|
| `0x70` | `SYSCALL` | `[OP]` | Triggers a system call. Syscall number in `B`, arguments and return in `A` |


## v0.4 (Memory)

| Opcode | Name    | Format            | Description                       |
|--------|---------|-------------------|-----------------------------------|
| `0x60` | `LOAD`  | `[OP][reg][addr]` | Loads data from addr to register  |
| `0x61` | `STORE` | `[OP][addr][reg]` | Writes data from register to addr |
