#include <stdint-gcc.h>
#include <stddef.h>

typedef struct page
{
	uint32_t used;
}page;

uint32_t ask_page();

void *ask_page_ptr();

void init_mem();

void use_page(uint32_t page_num);

void free_page(uint32_t page_num);

void free_ptr(void *ptr);

uint32_t ask_page();

void *ask_page_ptr();