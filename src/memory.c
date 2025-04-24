#include "../include/memory.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t mem_read(Memory *mem, uint16_t addr) {
  if ((int)addr >= MEMORY_SIZE) {
    printf("Error: memory read out of bounds (0x%02X)\n", addr);
    exit(1);
  };
  return mem->data[addr];
}

void mem_write(Memory *mem, uint16_t addr, uint8_t value) {
  if (addr >= TEXT_START && addr <= TEXT_END) {
    printf("Error: write to .text section (0x%02X)\n", addr);
    exit(1);
  }

  mem->data[addr] = value;
}