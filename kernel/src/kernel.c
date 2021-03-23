#include <terminal.h>
#include <string.h>
int _start()
{
    clear();
    char *s = "iwantyou123...........11113445667778";
    print_string(s,strlen(s));
    set_cur();
    for(;;){}
    return 0;
}