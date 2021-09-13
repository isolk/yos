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
    // 0000 0000 0001 0100
    t->ss0 = 0x14;
    t->esp0 = 1023;

    t->cs = 0x08;
    t->es=0x14;
    t->ss=0x14;
    t->ds=0x10;
    t->fs=0x14;
    t->gs=0x14;
    t->ldt_selector = 0x18;
    t->io=0;
}



void tss_func2(){

}