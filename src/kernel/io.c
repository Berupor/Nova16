
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

void kernel_print(CPU *cpu) { printf("%d\n", cpu->reg_a); }

void kernel_read(CPU *cpu) { scanf("%hhu", &cpu->reg_a); }

void kernel_exit(CPU *cpu) {
  printf("Program exited with code %d\n", cpu->reg_a);
  exit(cpu->reg_a);
}