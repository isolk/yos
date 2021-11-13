#include "pm.h"
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
	uint32_t p = ask_page();
	return p * 4096;
}