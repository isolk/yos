#include "mem.h"
void cp(uint8_t *s_addr, uint8_t *d_addr, uint32_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		asm volatile(
			"movb (%0),%%bl;"
			"movb %%bl,(%1);"
			:
			: "p"(s_addr + i), "p"(d_addr + i)
			: "%bx");
	}
}