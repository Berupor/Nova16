#include "cpu.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

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
  INC = 0x22,
  DEC = 0x23,
  CMP = 0x30,
  JMP = 0x40,
  JZ = 0x41,
  NOP = 0xF0,
  CALL = 0x50,
  RET = 0x51,
  PRINT = 0x60,
  READ = 0x61,
  HLT = 0xFF,
};

uint8_t fetch(CPU *cpu) { return cpu->memory[cpu->pc++]; }

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

    case INC: {
      uint8_t reg = fetch(cpu);

      uint8_t *reg_ptr = get_reg_ptr(cpu, reg);

      if (!reg_ptr) {
        printf("INC: unknown register %d at PC=%02X\n", reg, cpu->pc - 1);
        exit(1);
      }

      *reg_ptr += 1;
      break;
    }

    case DEC: {
      uint8_t reg = fetch(cpu);

      uint8_t *reg_ptr = get_reg_ptr(cpu, reg);

      if (!reg_ptr) {
        printf("DEC: unknown register %d at PC=%02X\n", reg, cpu->pc - 1);
        exit(1);
      }

      *reg_ptr -= 1;
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

    case CALL: {
      uint8_t subprogram_addr = fetch(cpu);

      uint8_t return_addr = cpu->pc;
      push(cpu, return_addr);

      cpu->pc = subprogram_addr;
      break;
    }

    case RET: {
      uint8_t return_addr = pop(cpu);

      cpu->pc = return_addr;
      break;
    }

    case PRINT: {
      uint8_t reg = fetch(cpu);
      printf("%d\n", *get_reg_ptr(cpu, reg));
      break;
    }

    case READ: {
      uint8_t reg = fetch(cpu);
      scanf("%hhu", get_reg_ptr(cpu, reg));
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