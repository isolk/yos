#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
uint8_t *bf;
int _start()
{
    bf = (uint8_t)0xb8000;
    bf[0] = 'h';
    bf[1]=0x70;
    return 0;
}