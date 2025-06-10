#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include <stdint.h>

enum {
  R0 = 0x00,
  R1 = 0x01,
  R2 = 0x02,
  R3 = 0x03,
};

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
  LOAD = 0x60,
  STORE = 0x61,
  SYSCALL = 0x70,
  HLT = 0xFF,
};

typedef struct {
  uint8_t r0;
  uint8_t r1;
  uint8_t r2;
  uint8_t r3;
  Memory *memory;
  uint16_t stack[STACK_SIZE];
  uint16_t sp;
  uint16_t pc;
  uint16_t zf;
  uint16_t hp;
} CPU;

uint8_t fetch8(CPU *cpu);
uint16_t fetch16(CPU *cpu);
void run(CPU *cpu);

#endif
