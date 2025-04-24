#include "cpu.h"
#include "debug.h"
#include "heap.h"
#include "kernel/kernel.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint8_t fetch8(CPU *cpu) { return mem_read(cpu->memory, cpu->pc++); }

uint16_t fetch16(CPU *cpu) {
  uint8_t lo = fetch8(cpu);
  uint8_t hi = fetch8(cpu);
  return (hi << 8) | lo; // little-endian
}

uint8_t *get_reg_ptr(CPU *cpu, uint8_t reg) {
  switch (reg) {
  case R0:
    return &cpu->r0;
  case R1:
    return &cpu->r1;
  case R2:
    return &cpu->r2;
  case R3:
    return &cpu->r3;
  default:
    return NULL;
  }
}

void run(CPU *cpu) {
  int running = 1;

  while (running) {
    uint8_t opcode = fetch8(cpu);
    if (DEBUG) {
      uint16_t pc_before = cpu->pc;
      debug_trace_opcode(cpu, opcode, pc_before);
    }

    switch (opcode) {

    case MOV: {
      uint8_t dst_reg = fetch8(cpu);
      uint8_t src_reg = fetch8(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (dst && src) {
        *dst = *src;
      }
      break;
    }

    case MOVI: {
      uint8_t reg = fetch8(cpu);
      uint8_t val = fetch8(cpu);
      uint8_t *target = get_reg_ptr(cpu, reg);

      if (target) {
        *target = val;
      }
      break;
    }

    case PUSH: {
      uint8_t reg_addr = fetch8(cpu);
      uint8_t *reg = get_reg_ptr(cpu, reg_addr);

      push(cpu, *reg);
      break;
    }

    case POP: {
      uint8_t reg_addr = fetch8(cpu);
      uint8_t *reg = get_reg_ptr(cpu, reg_addr);

      uint8_t value = pop(cpu);

      *reg = value;
      break;
    }

    case ADD: {
      uint8_t dst_reg = fetch8(cpu);
      uint8_t src_reg = fetch8(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (dst && src) {
        *dst += *src;
      }
      break;
    }

    case SUB: {
      uint8_t dst_reg = fetch8(cpu);
      uint8_t src_reg = fetch8(cpu);

      uint8_t *dst = get_reg_ptr(cpu, dst_reg);
      uint8_t *src = get_reg_ptr(cpu, src_reg);

      if (dst && src) {
        *dst -= *src;
      }
      break;
    }

    case INC: {
      uint8_t reg = fetch8(cpu);

      uint8_t *reg_ptr = get_reg_ptr(cpu, reg);

      if (!reg_ptr) {
        printf("INC: unknown register %d at PC=%02X\n", reg, cpu->pc - 1);
        exit(1);
      }

      *reg_ptr += 1;
      break;
    }

    case DEC: {
      uint8_t reg = fetch8(cpu);

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
      uint8_t dst_reg = fetch8(cpu);
      uint8_t src_reg = fetch8(cpu);

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
      uint16_t jmp_addr = fetch16(cpu);

      cpu->pc = jmp_addr;
      break;
    }

    case JZ: {
      uint16_t jmp_addr = fetch16(cpu);

      if (cpu->zf == 1) {
        cpu->pc = jmp_addr;
      }
      break;
    }

    case CALL: {
      uint16_t subprogram_addr = fetch16(cpu);
      uint16_t return_addr = cpu->pc;
      push(cpu, return_addr);
      cpu->pc = subprogram_addr;

      break;
    }

    case RET: {
      uint8_t return_addr = pop(cpu);

      cpu->pc = return_addr;
      break;
    }

    case LOAD: {
      uint8_t reg = fetch8(cpu);
      uint16_t addr = fetch16(cpu);
      uint8_t value = mem_read(cpu->memory, addr);

      uint8_t *target = get_reg_ptr(cpu, reg);

      if (target) {
        *target = value;
      }

      break;
    }

    case STORE: {
      uint16_t addr = fetch16(cpu);
      uint8_t reg = fetch8(cpu);

      uint8_t *src = get_reg_ptr(cpu, reg);
      if (src) {
        mem_write(cpu->memory, addr, *src);
      }
      break;
    }

    case SYSCALL: {
      uint8_t syscall_number = cpu->r1;
      kernel_handle(syscall_number, cpu, cpu->memory);

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