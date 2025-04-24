#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define TEXT_START 0x0000
#define TEXT_END 0x3FFF // 16K for code

#define DATA_START 0x4000
#define DATA_END 0x5FFF // 8K

#define HEAP_START 0x6000
#define HEAP_END 0x6FFF // 4K

#define STACK_START 0xFF00
#define STACK_END 0xFFFF // 256 bytes for stack

#define STACK_SIZE 256
#define MEMORY_SIZE 65536

typedef struct Memory {
  uint8_t data[MEMORY_SIZE];
} Memory;

uint8_t mem_read(Memory *mem, uint16_t addr);
void mem_write(Memory *mem, uint16_t addr, uint8_t value);

#endif
