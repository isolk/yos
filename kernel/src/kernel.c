#include <terminal.h>
#include <string.h>
int _start()
{
    clear();
    char r[25*80];
    for (size_t i = 0; i < sizeof(r); i++)
    {
        r[i]=i%10+'0';
    }
    
    char * hello = "hello\0";
    print_string(hello,strlen(hello));
    print_string(r,sizeof(r));
    print_string(hello,strlen(hello));

    for(;;){}
    return 0;
}