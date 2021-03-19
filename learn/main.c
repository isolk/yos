#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//uint8_t *bf;
int _start()
{
    uint8_t *pf = (uint8_t*)0xB8000;
    *pf = 'x';
    for(;;){}
    return 0;
}