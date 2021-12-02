#include "common.h"

void mem_copy(uint8_t *s_addr, uint8_t *d_addr, uint32_t count);
int brk(const void *addr);
void *sbrk(int incr);
