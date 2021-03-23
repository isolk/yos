#include <terminal.h>
#include <string.h>
int _start()
{
    clear();
    char *s = "hello,world.\nwhat you are!!!!!";
    print_string(s,strlen(s));
    set_cur();
    for(;;){}
    return 0;
}