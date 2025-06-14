#include "debug.h"
#include <stdio.h>

#define COLOR_RESET "\033[0m"
#define COLOR_REG "\033[38;5;75m"
#define COLOR_PC "\033[38;5;220m"
#define COLOR_STACK "\033[38;5;203m"
#define COLOR_OPCODE "\033[38;5;39m"
#define COLOR_HEADING "\033[1;37m"

int DEBUG_ENABLED = 0;
int TRACE_JSON_ENABLED = 0;

void debug_print_cpu(const CPU *cpu) {
  printf(COLOR_HEADING "\n=== CPU STATE ===\n" COLOR_RESET);

  printf(COLOR_PC "PC:  0x%04X  ", cpu->pc);
  printf("ZF: %d\n" COLOR_RESET, cpu->zf);

  printf(COLOR_REG "R0: %3d (0x%02X)   ", cpu->r0, cpu->r0);
  printf("R1: %3d (0x%02X)\n", cpu->r1, cpu->r1);
  printf("R2: %3d (0x%02X)   ", cpu->r2, cpu->r2);
  printf("R3: %3d (0x%02X)\n" COLOR_RESET, cpu->r3, cpu->r3);

  const int max_display = 12;
  int start = cpu->sp < max_display ? 0 : cpu->sp - max_display;

  printf(COLOR_STACK "\nStack (SP = %d):\n", cpu->sp);
  for (int i = STACK_SIZE - 1; i >= 0; i--) {
    if (i < start)
      continue;

    const char *marker = (i == cpu->sp) ? "→" : " ";
    printf("%s [%3d] = %3d\n", marker, i, cpu->stack[i]);
  }
  printf(COLOR_RESET);
}

void debug_trace_opcode(const CPU *cpu, uint8_t opcode, uint16_t pc_before) {
  printf(COLOR_OPCODE
         "[%04X] OPC=%02X R0=%d R1=%d R2=%d R3=%d SP=%d ZF=%d\n" COLOR_RESET,
         pc_before, opcode, cpu->r0, cpu->r1, cpu->r2, cpu->r3, cpu->sp,
         cpu->zf);
}

void json_dump_state(const CPU *cpu) {
  fprintf(stderr, "{\n");
  fprintf(stderr, "  \"registers\": {\n");
  fprintf(stderr, "    \"R0\": %d,\n", cpu->r0);
  fprintf(stderr, "    \"R1\": %d,\n", cpu->r1);
  fprintf(stderr, "    \"R2\": %d,\n", cpu->r2);
  fprintf(stderr, "    \"R3\": %d,\n", cpu->r3);
  fprintf(stderr, "    \"SP\": %d,\n", cpu->sp);
  fprintf(stderr, "    \"ZF\": %d,\n", cpu->zf);
  fprintf(stderr, "    \"PC\": %d\n", cpu->pc);
  fprintf(stderr, "  },\n");
  fprintf(stderr, "  \"stack\": [");
  for (int i = 0; i < STACK_SIZE; i++) {
    fprintf(stderr, "%d", cpu->stack[i]);
    if (i != STACK_SIZE - 1)
      fprintf(stderr, ", ");
  }
  fprintf(stderr, "],\n");
  fprintf(stderr, "  \"memory\": {");
  int first = 1;
  for (int i = 0; i < MEMORY_SIZE; i++) {
    if (cpu->memory->data[i] != 0) {
      if (!first)
        fprintf(stderr, ", ");
      fprintf(stderr, "\"0x%04X\": %d", i, cpu->memory->data[i]);
      first = 0;
    }
  }
  fprintf(stderr, "}\n");
  fprintf(stderr, "}\n");
}
