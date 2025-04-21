#include "cpu.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  CPU cpu = {0};
  Memory mem = {0};
  cpu.sp = STACK_SIZE;
  cpu.memory = &mem;

  FILE *f = fopen("bin/program.bin", "rb");
  if (!f) {
    perror("File open failed");
    exit(1);
  }

  uint8_t header[8];
  fread(header, 1, 8, f);

  if (header[0] != 0xDE || header[1] != 0xAD) {
    printf("Invalid binary format\n");
    exit(1);
  }

  uint8_t version = header[2];
  uint8_t entry_point = header[3];

  fread(cpu.memory->data, 1, MEMORY_SIZE, f);
  cpu.pc = entry_point;
  printf("%d PC\n", cpu.pc);

  fclose(f);

  run(&cpu);

  printf("----------\n");
  printf("reg_a: %d\n", cpu.reg_a);
  printf("reg_b: %d\n", cpu.reg_b);

  printf("stack: [");

  for (int i = 0; i < 16; i++) {
    printf("%d", cpu.stack[i]);
    if (i != 15) {
      printf(",");
    }
  }
  printf("]\n");
}
