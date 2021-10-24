#include "mem.h"

void *_s = 24 * 1024 * 1024;
void *_end = 0;
void *brk(const void *addr)
{
	_end = addr;
	return _end;
}

void *sbrk(int incr)
{
	_end = _s + incr;
	return _end;
}