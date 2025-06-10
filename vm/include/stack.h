#ifndef STACK_H
#define STACK_H

#include "cpu.h"
#include <stdint.h>

void push(CPU *cpu, uint8_t value);
uint8_t pop(CPU *cpu);

#endif
