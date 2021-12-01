#include <sys_call.h>>
#include "string.h"
#include "process.h"

void syscall_handler()
{
    int16_t num = 0;
    uint16_t pa1 = 0;
    asm(
        ""
        : "=a"(num), "=b"(pa1));
    if (num == 1)
    {
        print_char((char)pa1);
    }
    else if (num == 2)
    {
        exit_process();
    }
}
