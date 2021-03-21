#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <vga.h>
#include <terminal.h>

int _start()
{
    clear();
    char *s = "hello,world.\nwhat you are!!!!!";
    print_string(s,30);
    set_cur();
    for(;;){}
    return 0;
}