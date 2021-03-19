#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//uint8_t *bf;
int _start()
{
    *((int*)0xb8000)=0x07690748;
    return 0;
}