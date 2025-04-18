from enum import IntEnum

# FILE_PATH = "./programs/loop.v8asm"
# FILE_PATH = "./programs/loop_2.v8asm"
# FILE_PATH = "./programs/call.v8asm"
FILE_PATH = "./programs/add_x_y.v8asm"
# FILE_PATH = "./programs/io.v8asm"


class Registers(IntEnum):
    A = 0x00
    B = 0x01


class Opcodes(IntEnum):
    MOV = 0x01
    MOVI = 0x02
    PUSH = 0x10
    POP = 0x11
    ADD = 0x20
    SUB = 0x21
    INC = 0x22
    DEC = 0x23
    CMP = 0x30
    JMP = 0x40
    JZ = 0x41
    CALL = 0x50
    RET = 0x51
    PRINT = 0x60
    READ = 0x61
    NOP = 0xF0
    HLT = 0xFF


def get_register_addr_by_name(name: str) -> int:
    if name == Registers.A.name:
        return Registers.A.value
    elif name == Registers.B.name:
        return Registers.B.value
    else:
        raise Exception(f"unkown register: {name}")


def assembler_to_bytes():
    result = []

    f = open(FILE_PATH, "r")
    program_bytes = 0
    label_map = {}

    for line in f:
        line = line.strip()

        line_2 = line.replace(",", "")
        line_2 = line.split(" ")

        if line.endswith(":"):
            label_map[line.replace(":", "")] = program_bytes - 1
            continue

        program_bytes += len(line_2)

    f = open(FILE_PATH, "r")
    for line in f:
        line = line.strip()
        if not line or line.startswith(";"):
            continue

        line = line.replace(",", "")
        line = line.split(" ")

        opcode = line[0]

        if opcode == Opcodes.MOV.name:
            result.append(Opcodes.MOV.value)
            result.append(get_register_addr_by_name(line[1]))
            result.append(get_register_addr_by_name(line[2]))

        elif opcode == Opcodes.MOVI.name:
            result.append(Opcodes.MOVI.value)
            result.append(get_register_addr_by_name(line[1]))
            result.append(int(line[2], 0))

        elif opcode == Opcodes.PUSH.name:
            result.append(Opcodes.PUSH.value)
            result.append(get_register_addr_by_name(line[1]))

        elif opcode == Opcodes.POP.name:
            result.append(Opcodes.POP.value)
            result.append(get_register_addr_by_name(line[1]))

        elif opcode == Opcodes.ADD.name:
            result.append(Opcodes.ADD.value)
            result.append(get_register_addr_by_name(line[1]))
            result.append(get_register_addr_by_name(line[2]))

        elif opcode == Opcodes.SUB.name:
            result.append(Opcodes.SUB.value)
            result.append(get_register_addr_by_name(line[1]))
            result.append(get_register_addr_by_name(line[2]))

        elif opcode == Opcodes.INC.name:
            result.append(Opcodes.INC.value)
            result.append(get_register_addr_by_name(line[1]))

        elif opcode == Opcodes.DEC.name:
            result.append(Opcodes.DEC.value)
            result.append(get_register_addr_by_name(line[1]))

        elif opcode == Opcodes.CMP.name:
            result.append(Opcodes.CMP.value)
            result.append(get_register_addr_by_name(line[1]))
            result.append(get_register_addr_by_name(line[2]))

        elif opcode == Opcodes.JMP.name:
            result.append(Opcodes.JMP.value)
            try:
                result.append(int(line[1], 0))
            except ValueError:
                result.append(label_map[line[1]])

        elif opcode == Opcodes.JZ.name:
            result.append(Opcodes.JZ.value)
            try:
                result.append(int(line[1], 0))
            except ValueError:
                result.append(label_map[line[1]])

        elif opcode == Opcodes.CALL.name:
            result.append(Opcodes.CALL.value)
            try:
                result.append(int(line[1], 0))
            except ValueError:
                result.append(label_map[line[1]])

        elif opcode == Opcodes.RET.name:
            result.append(Opcodes.RET.value)

        elif opcode == Opcodes.PRINT.name:
            result.append(Opcodes.PRINT.value)
            result.append(get_register_addr_by_name(line[1]))

        elif opcode == Opcodes.READ.name:
            result.append(Opcodes.READ.value)
            result.append(get_register_addr_by_name(line[1]))

        elif opcode == Opcodes.NOP.name:
            result.append(Opcodes.NOP.value)

        elif opcode == Opcodes.HLT.name:
            result.append(Opcodes.HLT.value)

    print(result)
    print(label_map)
    f = open("bin/program.bin", "wb")
    f.write(bytes(result))
    f.close()


if __name__ == "__main__":
    assembler_to_bytes()
