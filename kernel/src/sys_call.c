#include<sys_call.h>>

void syscall_handler()
{
    uint16_t num = 0;
    uint16_t pa1 = 0;
    asm("mov %%ax,%0;mov %%bx,%1":"=r"(num),"=r"(pa1));
    if (num == 1){
        print_char((char)pa1);
    }
}
