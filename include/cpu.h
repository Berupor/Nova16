#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include <stdint.h>

#define MEMORY_SIZE 256
#define STACK_SIZE 16

typedef struct {
  uint8_t reg_a;
  uint8_t reg_b;
  Memory *memory;
  // uint8_t memory[MEMORY_SIZE];
  uint8_t stack[STACK_SIZE];
  uint8_t sp;
  uint8_t pc;
  uint8_t zf;
  uint8_t hp;
} CPU;

uint8_t fetch(CPU *cpu);
void run(CPU *cpu);
void push(CPU *cpu, uint8_t value);
uint8_t pop(CPU *cpu);

#endif
