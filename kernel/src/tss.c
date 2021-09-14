#include <tss.h>

tss tss_tables[256];
int tss_index = 0;

void tss_func1(){
    asm("xchg %bx,%bx");
    print_char('*');
    print_string("hello",5);
}

tss *new_tss(){
    return &tss_tables[tss_index++];
}

void init_tss1(tss * t){
    t -> io =1;
    t->tss =0;

    t->eip = tss_func1;
    t->eflags = 0;
    t->esp = 1023;

    // 0000 0000 0001 0000
    // 0000 0000 0001 1100
    // 01100
    t->ss0 = 0x0b1111;
    t->esp0 = 1023;

    t->cs = 0b01111;
    t->es=0b10111;
    t->ss=0b10111;
    t->ds=0b10111;
    t->fs=0b10111;
    t->gs=0b10111;
    t->ldt_selector = 0x18|0b11;
    t->io=0;
}



void tss_func2(){

}