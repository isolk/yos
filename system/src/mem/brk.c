#include "mem.h"

void *_s = 24 * 1024 * 1024;
void *_end = 24 * 1024 * 1024;
int brk(const void *addr)
{
	_end = addr;
	return 1;
}

void *sbrk(int incr)
{
	void *t = _end;
	_end += incr;
	return t;
}