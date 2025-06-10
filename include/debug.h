#ifndef DEBUG_H
#define DEBUG_H

#include "cpu.h"

extern int DEBUG_ENABLED;
extern int TRACE_JSON_ENABLED;

void debug_print_cpu(const CPU *cpu);
void debug_trace_opcode(const CPU *cpu, uint8_t opcode, uint16_t pc_before);
void json_dump_state(const CPU *cpu);

#endif
