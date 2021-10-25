#include "stdlib.h"
#include <mem.h>
// 将内核分配的空间，分配成多个块，每个块记录内部使用情况。
// 将所有的块记录下来即可。

static int init = 1;
malloc_chunk *start = 0;
malloc_chunk *end = 0;
// 初始化，请求4k，1k64，1k128，2k256
// 先测试1k的情况
void malloc_init()
{
	void *begin = sbrk(0);
	end = brk(begin + init_size);
	start = begin;

	for (size_t i = 0; i < 16 * 4; i++)
	{
		malloc_chunk *st = (void *)begin + 64 * i;
		st->size = 64;
		st->free = 1;
	}

	init = 0;
}

void *malloc(size_t size)
{
	if (init != 0)
	{
		malloc_init();
	}

	for (malloc_chunk *begin = start; begin != end; begin = (void *)begin + begin->size)
	{
		if (begin->free && size <= begin->size - 5)
		{
			begin->free = 0;
			return (void *)begin + 5;
		}
	}

	return NULL;
}

void free(void *addr)
{
	for (malloc_chunk *begin = start; begin != end; begin = (void *)begin + begin->size)
	{
		if (begin->free)
		{
			continue;
		}
		if (addr != (void *)begin + 5)
		{
			continue;
		}
		begin->free = 1;
		return;
	}
}