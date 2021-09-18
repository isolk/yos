#include <tss.h>

tss tss_tables[256];
int tss_index = 0;

void tss_func2(){
    for (size_t i = 0; i < 10000000; i++)
    {
        asm("int $0x80");
        // print_char('1'); // tss_func2只会在用户态执行，print_char是定义在内核段，直接访问会抛出io异常！
    }
}

void tss_func1(){
    for (size_t i = 0; i < 10000000; i++)
    {
        asm("int $0x80");
    }
}

tss *new_tss(){
    return &tss_tables[tss_index++];
}

void init_tss1(tss * t){
    t -> io =1;
    t->tss =0;

    t->eip = tss_func1;
    t->eflags = 0x200;
    t->esp = 1023;

    // 0000 0000 0001 0000
    // 0000 0000 0001 1100
    // 01100
    t->ss0 = 0b10000;
    t->esp0 = 4*1024*1024+1024;

    t->cs = 0b01111;
    t->es=0b10111;
    t->ss=0b10111;
    t->ds=0b10111;
    t->fs=0b10111;
    t->gs=0b10111;
    t->ldt_selector = 0x18|0b11;
    t->io=0x8000000;
}

void init_tss2(tss * t){
    t -> io =1;
    t->tss =0;

    t->eip = tss_func2;
    t->eflags = 0x200;
    t->esp = 1023;

    // 0000 0000 0001 0000
    // 0000 0000 0001 1100
    // 01100
    t->ss0 = 0b10000;
    t->esp0 = 4*1024*1024+2024;

    t->cs = 0b01111;
    t->es=0b10111;
    t->ss=0b10111;
    t->ds=0b10111;
    t->fs=0b10111;
    t->gs=0b10111;
    t->ldt_selector = 0x20|0b11;
    t->io=0x8000000;
}