// 管理所有的物理内存
// Note: This module is currently not implemented.
// Physical memory allocation is handled by pm.c
#include <stdint-gcc.h>
#include <stddef.h>

#define SIZE 131072

struct memory
{
    uint8_t bitmaps[SIZE]; //128K
};

struct memory me;

// TODO: Implement memory allocation using bitmap
void *mac(uint32_t size)
{
    // Not implemented yet
    (void)size;
    return NULL;
}
