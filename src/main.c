#include "../include/cpu.h"
#include "../include/memory.h"
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
  fread(cpu.memory, 1, MEMORY_SIZE, f);
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
