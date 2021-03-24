#include <terminal.h>
#include <string.h>
int _start()
{
    clear();
    char r[1111];
    for (size_t i = 0; i < sizeof(r); i++)
    {
        r[i]=i%10+'0';
    }
    
    print_string(r,sizeof(r));

    for(;;){}
    return 0;
}