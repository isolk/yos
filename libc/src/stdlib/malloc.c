#include "stdlib.h"
#include <mem.h>
// 将内核分配的空间，分配成多个块，每个块记录内部使用情况。
// 将所有的块记录下来即可。

static int init = 1;
malloc_chunk *start = 0;
malloc_chunk *end = 0;
// 初始化，请求4k，1k64，1k128，2k256
malloc_chunk *next_malloc(malloc_chunk *it)
{
	return (void *)it + it->size;
}

void init_chunks(malloc_chunk *it, malloc_chunk *end, uint32_t size)
{
	while (it != end)
	{
		it->size = size;
		it->free = 1;
		it = next_malloc(it);
	}
}

void malloc_init()
{
	start = sbrk(init_size);
	end = sbrk(0);
	init_chunks(start, end, block_size);
	init = 0;
}

_bool tryMerge(malloc_chunk *orgin, uint32_t size)
{
	uint32_t totalSize = 0;
	for (malloc_chunk *it = orgin; it != end && it->free; it = next_malloc(it))
	{
		totalSize += it->size;
		if (totalSize >= size)
		{
			orgin->size = totalSize;
			return TRUE;
		}
	}
	return FALSE;
}

void *alloc(malloc_chunk *mc)
{
	mc->free = 0;
	return &(mc->data);
}

// 请求分配至少这么多字节的空间，
void *ask_init(uint32_t size)
{
	uint32_t newSeg = 0;
	malloc_chunk *newBegin = NULL;
	if (size < _size_64)
	{
		newSeg = _size_64;
		newBegin = sbrk(_size_64 * 32);
	}
	else if (size < _size_1k)
	{
		newSeg = _size_1k;
		newBegin = sbrk(_size_1k * 16);
	}
	else if (size < _size_4k)
	{
		newSeg = _size_4k;
		newBegin = sbrk(_size_4k * 8);
	}
	else
	{
		uint32_t s = _size_4k * 2;
		while (s < size)
		{
			s *= 2;
		}
		newSeg = s;
		newBegin = sbrk(newSeg * 4);
	}
	if (newBegin == NULL)
	{
		return NULL;
	}

	end = sbrk(0);
	init_chunks(newBegin, end, newSeg);
	return newBegin;
}

// 算法：
// 先找满足的，找到就返回
// 没找到，判断可以合并多少个连续的，如果满足，合并并且返回
// 如果合并都不满足的，请求新的块，大小为目标大小*2，然后返回
//
// 算法缺点：
// - 分配慢，每次都要遍历，并且要经历合并的尝试，效率低
// - 内存碎片可能会比较多。比如当前块大小为8K，请求的为4B，也会立即返回。
// 后面有时间再考虑优化吧，比如对大块进行拆分、添加固定大小的块链表等。目前能用就行。
void *malloc(size_t size)
{
	if (init != 0)
	{
		malloc_init();
	}

	// 先找满足需求的块
	for (malloc_chunk *it = start; it != end; it = next_malloc(it))
	{
		if (it->free && size <= it->size - 5)
		{
			return alloc(it);
		}
	}

	// 尝试合并
	for (malloc_chunk *it = start; it != end; it = next_malloc(it))
	{
		if (tryMerge(it, size))
		{
			return alloc(it);
		}
	}

	// 重新申请
	malloc_chunk *newBegin = ask_init(size);
	return alloc(newBegin);
}

void free(void *addr)
{
	for (malloc_chunk *it = start; it != end; it = next_malloc(it))
	{
		if (addr != &(it->data))
		{
			continue;
		}
		it->free = 1;
		trySplit(it);
		return;
	}
}

// 合并策略，归还时，总是会尝试将所有的块合并成64B的小块
void trySplit(malloc_chunk *it)
{
	uint32_t size = it->size;
	for (size_t i = 0; i < size; i += block_size)
	{
		it->size = block_size;
		it->free = 1;
		it = next_malloc(it);
	}
}