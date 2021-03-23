#include <string.h>
void *memcpy(void *dst, const void *src, size_t len)
{
    size_t i;
    char *d = dst;
    const char *s = src;
    for (i = 0; i < len; i++)
    {
        d[i] = s[i];
    }
    return dst;
}