#ifndef DEBUG_H
#define DEBUG_H

#include "cpu.h"

#define DEBUG 1

void debug_print_cpu(const CPU *cpu);
void debug_trace_opcode(const CPU *cpu, uint8_t opcode, uint16_t pc_before);

#endif
