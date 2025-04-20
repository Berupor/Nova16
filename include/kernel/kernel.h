#ifndef KERNEL_H
#define KERNEL_H

#include "cpu.h"
#include "memory.h"

void kernel_handle(uint8_t syscall_number, CPU *cpu, Memory *memory);

#endif
