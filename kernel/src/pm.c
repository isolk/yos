#include "pm.h"
typedef struct ktrunk
{
	uint8_t used;
	uint32_t byte_size;
} __attribute__((packed)) ktrunk;

ktrunk *km;		  // 1k大小 1G=1MB
page *page_table; // 通过init_mem设置位置
uint32_t page_size = 0;

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
	return end_addr / 4096 + 1;
}

void init_page_table_mem(uint32_t begin_page)
{
	page_table = begin_page * 4096;
}

void init_mem()
{
	page_size = get_total_mem_k() / 4 + 1;
	uint32_t kernel_end_page = get_kernel_end_page();
	init_page_table_mem(kernel_end_page + 1);

	for (size_t i = 0; i < page_size; i++)
	{
		page_table[i].used = 0;
	}

	uint32_t cost_page = page_size * sizeof(page) / 4096 + 1; //  计算page_table[]需要多少页来容纳
	for (size_t i = 0; i < kernel_end_page + cost_page; i++)
	{
		page_table[i].used = 1;
	}

	km = (kernel_end_page + cost_page + 512) * 4096;
	km->used = 0;
	km->byte_size = (page_size - kernel_end_page - cost_page - 512) * 4096;
}

void use_page(uint32_t page_num)
{
	page_table[page_num].used = 1;
}

void free_page(uint32_t page_num)
{
	page_table[page_num].used = 0;
}

void free_ptr(void *ptr)
{
	uint32_t page_num = (uint32_t)ptr / 4096;
	free_page(page_num);
}

uint32_t ask_page()
{
	for (size_t i = 0; i < page_size; i++)
	{
		if (!page_table[i].used)
		{
			page_table[i].used = 1;
			return i;
		}
	}
	return -1;
}

void *ask_page_ptr()
{
	return kalloc_frame(1);
	// uint32_t p = ask_page();
	// return p * 4096;
}

#define data(it) (uint32_t) it + sizeof(ktrunk)
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
	ktrunk *next_it = next(it);
	next_it->byte_size = it->byte_size - size - sizeof(ktrunk);
	next_it->used = 0;

	it->used = 1;
	it->byte_size = size;
}
