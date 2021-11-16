#include "pm.h"
#include "string.h"
typedef struct ktrunk
{
	uint8_t used;
	uint32_t byte_size;
} __attribute__((packed)) ktrunk;

ktrunk *km; // 1k大小 1G=1MB

uint16_t get_total_mem_k()
{
	uint16_t *ptr_page_64k = 0x7c00 - 20;
	uint16_t high_mem_kb = *ptr_page_64k * 64;
	uint16_t *ptr_page_1k = 0x7c00 - 10;
	uint16_t low_mem_kb = *ptr_page_1k;
	return high_mem_kb + low_mem_kb;
}

uint32_t get_kernel_end_page()
{
	uint32_t *ptr_end_addr = 0x7c00 - 30;
	uint32_t end_addr = *ptr_end_addr;
	return (end_addr - 3 * 1024 * 1024 * 1024) / 4096 + 1;
}

void init_mem()
{
	uint32_t page_size = get_total_mem_k();
	uint32_t kernel_end_page = get_kernel_end_page();

	km = kernel_end_page * 4096;
	km->used = 0;
	km->byte_size = page_size * 1024 - kernel_end_page * 4096;
}

#define data(it) ((uint32_t)it + sizeof(ktrunk))
#define next(it) (uint32_t) it + sizeof(ktrunk) + it->byte_size
void *kalloc(size_t size)
{
	ktrunk *it = km;
	while (it->used || it->byte_size < size)
	{
		it = next(it);
	}
	split(it, size);
	return data(it);
}

ktrunk *split_frame(ktrunk *it, uint32_t frame_size)
{
	uint32_t addr = data(it);
	uint32_t cur_page_size = 4096 - addr % 4096;
	ktrunk *next_it = addr + cur_page_size - sizeof(ktrunk);

	next_it->byte_size = frame_size * 4096;
	next_it->used = 1;

	ktrunk *nn_next = next(next_it);
	nn_next->byte_size = it->byte_size - frame_size * 4096 - sizeof(ktrunk) - cur_page_size;
	nn_next->used = 0;

	it->used = 0;
	it->byte_size = cur_page_size - sizeof(ktrunk);
	return next_it;
}

void *kalloc_frame(uint32_t frame_size)
{
	ktrunk *it = km;
	while (it)
	{
		if (it->used)
		{
			it = next(it);
			continue;
		}
		if (it->byte_size < frame_size * 4096)
		{
			it = next(it);
			continue;
		}
		uint32_t addr = (uint32_t)it + sizeof(ktrunk);
		if (it->byte_size - addr % 4096 - 64 < frame_size * 4096) // zhiaoshao 64
		{
			it = next(it);
			continue;
		}
		ktrunk *ret = split_frame(it, frame_size);
		return (uint32_t)ret + sizeof(ktrunk);
	}
}

void split(ktrunk *it, size_t size)
{
	ktrunk *next_it = (uint32_t)it + sizeof(ktrunk) + size;
	next_it->byte_size = it->byte_size - size - sizeof(ktrunk);
	next_it->used = 0;

	it->used = 1;
	it->byte_size = size;
}

#define M (1024 * 1024)
#define K 1024
#define get_m(t) (t >> 20)
#define get_k(t) (t << 12 >> 22)
#define get_b(t) (t << 22 >> 22)
void print_mm()
{
	uint32_t total = 0;
	uint32_t used = 0;
	uint32_t left = 0;

	ktrunk *it = km;
	while (it->byte_size > 0)
	{
		uint32_t m = get_m(it->byte_size);
		uint32_t k = get_k(it->byte_size);
		uint32_t b = get_b(it->byte_size);

		uint32_t sm = get_m(data(it));
		uint32_t sk = get_k(data(it));
		uint32_t sb = get_b(data(it));

		total += it->byte_size;
		if (it->used)
		{
			used += it->byte_size;
		}
		else
		{
			left += it->byte_size;
		}

		printf("[start:{%dm,%dk,%db} size:{%dm,%dk,%db} used:{%d}]\n", sm, sk, sb, m, k, b, it->used);
		it = next(it);
	}

	printf("-----------total:{%dm,%dk,%db},used:{%dm,%dk,%db},left:{%dm,%dk,%db}--------------\n", get_m(total), get_k(total), get_b(total), get_m(used), get_k(used), get_b(used), get_m(left), get_k(left), get_b(left));
}

void kfree(void *addr)
{
	ktrunk *it = addr - sizeof(ktrunk);
	it->used = 0;
	gc();
}

ktrunk *merge(ktrunk *it)
{
	ktrunk *nit = next(it);
	if (nit->byte_size == 0)
	{
		return NULL;
	}

	if (it->used)
	{
		return nit;
	}

	if (nit->used)
	{
		return nit;
	}

	it->byte_size += nit->byte_size + sizeof(ktrunk);
	return it;
}

// 进行内存块合并处理
void gc()
{
	ktrunk *it = km;
	while (it != NULL)
	{
		it = merge(it);
	}
}