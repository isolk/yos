#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
int _start()
{
    uint8_t *pf = (uint8_t*)0xB8000;
    char *s = "hello,world";
    for (size_t i = 0; i < 11; i++)
    {
        *pf = s[i];
        pf +=2;
    }
    
    for(;;){}
    return 0;
}