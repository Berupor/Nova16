
#include "cpu.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>

void kernel_print(CPU *cpu) {
  if (DEBUG_ENABLED) {
    printf("\033[1;36m[SYSCALL print]\033[0m %d\n", cpu->r0);
    return;
  }
  printf("%d\n", cpu->r0);
}

void kernel_read(CPU *cpu) {
  if (DEBUG_ENABLED) {
    printf("\033[1;36m[SYSCALL read]\033[0m > ");
  }
  scanf("%hhu", &cpu->r0);
}

void kernel_exit(CPU *cpu) {
  if (DEBUG_ENABLED) {
    printf("\033[1;36m[SYSCALL exit]\033[0m Program exited with code %d\n",
           cpu->r0);
  } else {
    printf("Program exited with code %d\n", cpu->r0);
  }

  exit(cpu->r0);
}