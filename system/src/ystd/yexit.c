#include "ystd.h"

void yexit(uint16_t status)
{
	asm("xchg %bx,%bx");
	asm volatile(
		"mov $2,%%ax;"
		"mov %0,%%bx;"
		"int $0x80;"
		:
		: "r"(status)
		: "%ax", "%bx");
}