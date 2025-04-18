#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define TEXT_START 0x00
#define TEXT_END 0x7F

#define DATA_START 0x80
#define DATA_END 0xBF

#define HEAP_START 0xC0
#define HEAP_END 0xCF

#define STACK_START 0xF0
#define STACK_END 0xFF

#define MEMORY_SIZE 256

typedef struct Memory {
  uint8_t data[MEMORY_SIZE];
} Memory;

uint8_t mem_read(Memory *mem, uint8_t addr);
void mem_write(Memory *mem, uint8_t addr, uint8_t value);

#endif
