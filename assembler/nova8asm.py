from enum import IntEnum
from time import sleep

INPUT_FILE = "./programs/main.v8asm"

OUTPUT_FILE = "./bin/program.bin"


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
    SYSCALL = 0x70
    NOP = 0xF0
    HLT = 0xFF


def get_register_addr_by_name(name: str) -> int:
    if name == Registers.A.name:
        return Registers.A.value
    elif name == Registers.B.name:
        return Registers.B.value
    else:
        raise Exception(f"unkown register: {name}")


def create_label_map(files: list[str]):
    program_bytes = 0
    label_map = {}

    for filename in files:
        f = open(filename, "r")

        for line in f:
            line = line.strip()
            if line.startswith("INCLUDE") or line.startswith("ENTRY"):
                continue

            line_2 = line.replace(",", "")
            line_2 = line.split(" ")

            if line.endswith(":"):
                label = line.replace(":", "")
                label_map[label] = program_bytes
                continue

            if line_2 != [""]:
                program_bytes += len(line_2)

        f.close()

    return label_map


def assemble_file_to_bytes(filename: str, label_map: dict):
    result = []
    f = open(filename, "r")
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

        elif opcode == Opcodes.SYSCALL.name:
            result.append(Opcodes.SYSCALL.value)

        elif opcode == Opcodes.NOP.name:
            result.append(Opcodes.NOP.value)

        elif opcode == Opcodes.HLT.name:
            result.append(Opcodes.HLT.value)

    return result


def assembler_to_bytes():
    includes = []
    with open(INPUT_FILE, "r") as f:
        lines = f.readlines()

        program_bytes = 0
        label_map = {}

        for line in lines:
            line = line.strip()
            if not line or line.startswith(";"):
                continue

            if line.startswith("INCLUDE"):
                path = line.split(" ", 1)[1].strip().strip('"')
                includes.append(path)
                continue

            tokens = line.replace(",", "").split()

            if tokens[0].endswith(":"):
                label = tokens[0].replace(":", "")
                label_map[label] = program_bytes
                continue

            program_bytes += len(tokens)

    label_map = create_label_map(includes + [INPUT_FILE])

    result = []
    for filename in includes + [INPUT_FILE]:
        result.extend(assemble_file_to_bytes(filename, label_map))

    with open(INPUT_FILE, "r") as f:
        lines = f.readlines()

        for line in lines:
            line = line.strip()
            if line.startswith("ENTRY"):
                line = line.split(": ")
                ENTRY = label_map[line[1]]

    output_bytes = bytearray()
    # MAGIC_1
    output_bytes.append(0xDE)
    # MAGIC_2
    output_bytes.append(0xAD)
    # VERSION
    output_bytes.append(0x01)
    # ENTRY
    output_bytes.append(ENTRY)
    # CODESIZE
    output_bytes.append(len(result))
    # RESERVED
    output_bytes += bytes([0x00, 0x00, 0x00])

    output_bytes.extend(result)
    with open(OUTPUT_FILE, "wb") as f:
        f.write(output_bytes)

    hex_output = " ".join([f"{byte:02x}" for byte in output_bytes])
    print(label_map)
    print(ENTRY)
    print(hex_output)


if __name__ == "__main__":
    assembler_to_bytes()
