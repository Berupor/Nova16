#include "cpu.h"
#include <stdio.h>

void push(CPU *cpu, uint8_t value) {
  if (cpu->sp == 0) {
    printf("Stack overflow\n");
    return;
  }
  cpu->sp--;
  cpu->stack[cpu->sp] = value;
}

uint8_t pop(CPU *cpu) {
  if (cpu->sp == STACK_SIZE) {
    printf("Stack underflow\n");
    return 0;
  }
  return cpu->stack[cpu->sp++];
}