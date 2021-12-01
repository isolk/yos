#include <stdint.h>
#include <stddef.h>
#include "def.h"
#define _size_64 64
#define _size_1k 1024
#define _size_4k 4096
#define init_size 4096
#define block_size 64

typedef struct malloc_chunk
{
	uint32_t size; // 当前块大小
	uint8_t free;  // 当前块是否空闲
	void *data;
} __attribute__((packed)) malloc_chunk;

void *malloc(size_t size);
void free(void *addr);