#ifndef HEAP_H
#define HEAP_H

#include "memory.h"
#include <stdint.h>

uint8_t heap_alloc(Memory *memory, uint8_t size);
void heap_free(Memory *memory,uint8_t ptr);
void heap_dump(Memory *memory);

#endif
