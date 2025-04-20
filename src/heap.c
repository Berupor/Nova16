#include "../include/memory.h"
#include <stdint.h>
#include <stdio.h>

#define HEADER_SIZE 2
#define FREE_FLAG 0
#define OCCUPIED_FLAG 1

// ANSI цвета
#define COLOR_USED "\x1b[31m" // Красный
#define COLOR_FREE "\x1b[32m" // Зелёный
#define COLOR_RESET "\x1b[0m"

// TODO: добавить пару коментов логики
uint8_t heap_alloc(Memory *memory, uint8_t size) {
  uint8_t current_point = HEAP_START;

  while (current_point < HEAP_END) {
    uint8_t block_is_occupied = mem_read(memory, current_point);
    uint8_t block_size = mem_read(memory, current_point + 1);

    if (size <= block_size && !block_is_occupied) {
      mem_write(memory, current_point, OCCUPIED_FLAG);

      if (size < block_size - HEADER_SIZE) {
        uint8_t new_block_size = block_size - size - HEADER_SIZE;
        uint8_t new_block_size_is_occupied_addr =
            current_point + size + HEADER_SIZE;
        uint8_t new_block_size_addr = new_block_size_is_occupied_addr + 1;

        mem_write(memory, new_block_size_is_occupied_addr, FREE_FLAG);
        mem_write(memory, new_block_size_addr, new_block_size);
      }
      return current_point + HEADER_SIZE;

    } else {
      current_point += block_size + HEADER_SIZE;
    }
  }

  // TODO: Константа?
  return 0x00;
}

// TODO: Добавить склеивание слева?
void heap_free(Memory *memory, uint8_t ptr) {
  uint8_t header_ptr = ptr - 2;

  mem_write(memory, header_ptr, FREE_FLAG);

  // Начинаем склеивать
  uint8_t block_is_occupied = mem_read(memory, header_ptr);
  uint8_t block_size = mem_read(memory, header_ptr + 1);

  while (1) {
    uint8_t next_block_ptr = header_ptr + 2 + block_size;
    if (next_block_ptr >= HEAP_END) {
      break;
    }

    uint8_t next_block_is_free = mem_read(memory, next_block_ptr);
    if (!next_block_is_free) {
      break;
    }
    uint8_t next_block_size = mem_read(memory, next_block_ptr + 1);
    uint8_t old_size = mem_read(memory, header_ptr + 1);
    block_size = old_size + next_block_size + 2;

    mem_write(memory, header_ptr + 1, old_size + next_block_size + 2);
  }
}

void heap_dump(Memory *memory) {
  uint8_t current_point = HEAP_START;

  while (current_point < HEAP_END) {
    uint8_t block_is_occupied = mem_read(memory, current_point);
    uint8_t block_size = mem_read(memory, current_point + 1);

    const char *status = block_is_occupied ? "USED" : "FREE";
    const char *color = block_is_occupied ? COLOR_USED : COLOR_FREE;

    printf("%s0x%02X: [%s] size=%d%s", color, current_point, status, block_size,
           COLOR_RESET);

    if (block_is_occupied) {
      printf("  data: [");
      for (int i = 0; i < block_size; i++) {
        printf("%02X", mem_read(memory, current_point + HEADER_SIZE + i));
        if (i != block_size - 1)
          printf(" ");
      }
      printf("]");
    }
    printf("\n");

    current_point += block_size + HEADER_SIZE;
  }
}
