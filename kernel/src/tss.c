#include <tss.h>

tss tss_tables[256];
int tss_index = 0;

void tss_func1(){
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
    t->esp = 4*1024*1024*1024;

    t->ss0 = 0x10;
    t->esp0 = 4*1024*1024*1024;

    t->ss = 0x10;
    t->cs = 0x08;
    t->es=0x10;
    t->ss=0x10;
    t->ds=0x10;
    t->fs=0x10;
    t->gs=0x10;
    t->ldt_selector = 0x18;
    t->io=0;
}



void tss_func2(){

}