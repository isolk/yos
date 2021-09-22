#include <stdarg.h>
#include <stddef.h>
#define printf printf_
size_t strlen(const char *str);
void *memcpy(void *dest, const void * src, size_t n);
int printf_(const char* format, ...);