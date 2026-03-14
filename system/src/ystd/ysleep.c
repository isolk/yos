#include "ystd.h"

void ysleep(uint32_t ms)
{
	asm volatile(
		"mov $3,%%ax;"
		"mov %0,%%ebx;"
		"int $0x80;"
		:
		: "r"(ms)
		: "%ax", "%ebx");
}