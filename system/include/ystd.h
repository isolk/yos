#include "common.h"

void yexit(uint16_t status);

void ysleep(uint32_t ms);

static inline void sleep(uint32_t ms)
{
	ysleep(ms);
}