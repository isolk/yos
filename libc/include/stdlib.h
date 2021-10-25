#include <stdint.h>
#include <stddef.h>
#define init_size 4096

typedef struct malloc_chunk
{
	uint32_t size; // 当前块大小
	uint8_t free;  // 当前块是否空闲
	void *data;
} malloc_chunk;

void *malloc(size_t size);
void free(void *addr);