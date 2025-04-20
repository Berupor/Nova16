#include "kernel/kernel.h"
#include "kernel/io.h"

#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

enum {
  PRINT = 0x01,
  READ = 0x02,
  EXIT = 0x03,
};

void kernel_handle(uint8_t syscall_number, CPU *cpu, Memory *memory) {
  switch (syscall_number) {
  case PRINT:
    kernel_print(cpu);
    break;
  case READ:
    kernel_read(cpu);
    break;
  case 0x03:
    kernel_exit(cpu);
    break;

  default:
    printf("Unknown syscall: 0x%02X\n", syscall_number);
    exit(1);
  }
}
