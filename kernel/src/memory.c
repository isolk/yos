// 管理所有的物理内存
#include <stdint-gcc.h>
#include <stddef.h>
#define SIZE 131072
struct memory
{
    uint8_t bitmaps[SIZE]; //128K
};

struct memory me;

void *mac(uint32_t size)
{
    for (size_t i = 0; i < SIZE; i++)
    {
    }
}
