#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MEMORY_SIZE = 256;
const int STACK_SIZE = 16;

typedef struct CPU {
  uint8_t reg_a;
  uint8_t reg_b;
  uint8_t memory[MEMORY_SIZE];
  uint8_t stack[STACK_SIZE];
  uint8_t sp;
  uint8_t pc;
  uint8_t zf;
} CPU;

// Registers
enum { REG_A = 0x00, REG_B = 0x01 };

// Opcodes
enum {
  MOV = 0x01,
  MOVI = 0x02,
  PUSH = 0x10,
  POP = 0x11,
  ADD = 0x20,
  SUB = 0x21,
  CMP = 0x30,
  JMP = 0x40,
  JZ = 0x41,
  NOP = 0xF0,
  HLT = 0xFF,
};

uint8_t *get_reg_ptr(CPU *cpu, uint8_t reg) {
  switch (reg) {
  case REG_A:
    return &cpu->reg_a;
  case REG_B:
    return &cpu->reg_b;
  default:
    return NULL;
  }
}

uint8_t fetch(CPU *cpu) { return cpu->memory[cpu->pc++]; }

void push(CPU *cpu, uint8_t value) {
  if (cpu->sp == 0) {
    printf("Stack overflow\n");
    return;
  }

  cpu->sp--;
  cpu->stack[cpu->sp] = value;
}

uint8_t pop(CPU *cpu) {
  if (cpu->sp == 16) {
    printf("Stack underflow\n");
    return 0;
  }

  return cpu->stack[cpu->sp++];
}

void run(CPU *cpu) {
  int running = 1;

  while (running) {
    uint8_t opcode = fetch(cpu);
    printf("PC=%02X OPCODE=%02X A=%d B=%d SP=%d ZF=%d\n", cpu->pc - 1, opcode,
           cpu->reg_a, cpu->reg_b, cpu->sp, cpu->zf);

    switch (opcode) {

    case MOV: {
      uint8_t dst_reg = fetch(cpu);
      uint8_t src_reg = fetch(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (dst && src) {
        *dst = *src;
      }
      break;
    }

    case MOVI: {
      uint8_t reg = fetch(cpu);
      uint8_t val = fetch(cpu);
      uint8_t *target = get_reg_ptr(cpu, reg);

      if (target) {
        *target = val;
      }
      break;
    }

    case PUSH: {
      uint8_t reg_addr = fetch(cpu);
      uint8_t *reg = get_reg_ptr(cpu, reg_addr);

      push(cpu, *reg);
      break;
    }

    case POP: {
      uint8_t reg_addr = fetch(cpu);
      uint8_t *reg = get_reg_ptr(cpu, reg_addr);

      uint8_t value = pop(cpu);

      *reg = value;
      break;
    }

    case ADD: {
      uint8_t dst_reg = fetch(cpu);
      uint8_t src_reg = fetch(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (dst && src) {
        *dst += *src;
      }
      break;
    }

    case SUB: {
      uint8_t dst_reg = fetch(cpu);
      uint8_t src_reg = fetch(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (dst && src) {
        *dst -= *src;
      }
      break;
    }

    case CMP: {
      // TODO: Другие названия
      uint8_t dst_reg = fetch(cpu);
      uint8_t src_reg = fetch(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (*dst == *src) {
        cpu->zf = 1;
      } else {
        cpu->zf = 0;
      }
      break;
    }

    case JMP: {
      uint8_t jmp_addr = fetch(cpu);

      cpu->pc = jmp_addr;
      break;
    }

    case JZ: {
      uint8_t jmp_addr = fetch(cpu);

      if (cpu->zf == 1) {
        cpu->pc = jmp_addr;
      }
      break;
    }

    case NOP: {
      break;
    }

    case HLT: {
      running = 0;
      break;
    }

    default: {
      running = 0;
      break;
    }
    }
  }
}

int main() {
  CPU cpu = {0};
  cpu.sp = STACK_SIZE;

  FILE *f = fopen("program.bin", "rb");
  if (!f) {
    perror("File open failed");
    exit(1);
  }
  fread(cpu.memory, 1, MEMORY_SIZE, f);
  fclose(f);

  run(&cpu);

  printf("----------\n");
  printf("reg_a: %d\n", cpu.reg_a);
  printf("reg_b: %d\n", cpu.reg_b);

  printf("stack: [");

  for (int i = 0; i < 16; i++) {
    printf("%d", cpu.stack[i]);
    if (i != 15) {
      printf(",");
    }
  }
  printf("]\n");
}
