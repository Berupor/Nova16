#include "cpu.h"
#include "debug.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAGIC_1 0xDE
#define MAGIC_2 0xAD
#define HEADER_SIZE 10

#define ENTRY_LO 3
#define ENTRY_HI 4
#define SIZE_LO 5
#define SIZE_HI 6

const char *binary_path = NULL;

void load_binary(const char *path, CPU *cpu, uint16_t *entry_point) {

  FILE *f = fopen(path, "rb");
  if (!f) {
    perror("File open failed");
    exit(1);
  }

  uint8_t header[HEADER_SIZE];
  fread(header, 1, HEADER_SIZE, f);

  if (header[0] != MAGIC_1 || header[1] != MAGIC_2) {
    fprintf(stderr, "Invalid binary format\n");
    exit(1);
  }

  uint8_t version = header[2];
  *entry_point = header[ENTRY_LO] | (header[ENTRY_HI] << 8);
  uint16_t code_size = header[SIZE_LO] | (header[SIZE_HI] << 8);

  if (code_size > (TEXT_END - TEXT_START + 1)) {
    fprintf(stderr,
            "Program code too large (%d bytes), TEXT segment is %d bytes\n",
            code_size, TEXT_END - TEXT_START + 1);
    exit(1);
  }

  if (*entry_point < TEXT_START || *entry_point > TEXT_END) {
    fprintf(stderr, "Entry point 0x%04X is outside of TEXT segment\n",
            *entry_point);
    exit(1);
  }

  fread(cpu->memory->data + TEXT_START, 1, code_size, f);
  fclose(f);
}

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--trace-json") == 0) {
      TRACE_JSON_ENABLED = 1;
    } else if (strcmp(argv[i], "--debug") == 0) {
      DEBUG_ENABLED = 1;
    } else if (!binary_path) {
      binary_path = argv[i];
    }
  }

  if (!binary_path) {
    fprintf(stderr, "Usage: %s <program.novabin> [--debug] [--trace-json]\n",
            argv[0]);
    return 1;
  }

  CPU cpu = {0};
  Memory mem = {0};
  cpu.sp = STACK_SIZE;
  cpu.memory = &mem;

  uint16_t entry_point = 0;
  load_binary(binary_path, &cpu, &entry_point);
  cpu.pc = entry_point;

  if (DEBUG_ENABLED) {
    printf("\n\033[1;34m=== Nova VM Output ===\033[0m\n");
    printf("\033[1;32m✓ Program loaded successfully\033[0m\n");
    printf("ENTRY POINT: 0x%04X\n", cpu.pc);
    printf("Starting execution...\n\n");
  }

  run(&cpu);

  if (TRACE_JSON_ENABLED) {
    json_dump_state(&cpu);
  }

  if (DEBUG_ENABLED) {
    debug_print_cpu(&cpu);
  };

  return 0;
}