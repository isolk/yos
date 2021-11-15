#include <tss.h>
#include "page.h"
#include "string.h"
#include "pm.h"

tss tss_tables[256];

void kernel_run()
{
    char *list = kalloc(100);
    list[0] = 'y';
    list[1] = 'o';
    list[2] = 's';
    list[3] = '\0';
    while (1)
    {
        printf("%s\n", list);
        asm("hlt");
    }
}

void init_tss_kernel(uint32_t cr3)
{
    tss *t = &tss_tables[0];
    t->io = 1;
    t->tss = 0;

    t->eip = kernel_run;
    t->eflags = 0x200;
    t->esp = kalloc(4096) + 4096;

    t->cs = 0b00100;
    t->es = 0b01100;
    t->ss = 0b01100;
    t->ds = 0b01100;
    t->fs = 0b01100;
    t->gs = 0b01100;
    t->ldt_selector = 0x18;
    t->io = 0x8000000;
    t->cr3 = cr3;
}

void init_tss_user(tss *t, uint32_t cr3)
{
    t->io = 1;
    t->tss = 0;

    t->eip = kernel_run;
    t->eflags = 0x200;
    t->esp = 3 * 1024 * 1024 + 2048;

    // 0000 0000 0001 0000
    // 0000 0000 0001 1100
    // 01100
    // t->ss0 = 0b10000;
    // t->esp0 = 2 * 1024 * 1024 + 2024;

    t->cs = 0b01100;
    t->es = 0b10100;
    t->ss = 0b10100;
    t->ds = 0b10100;
    t->fs = 0b10100;
    t->gs = 0b10100;
    t->ldt_selector = 0x20 | 0b00;
    t->io = 0x8000000;
    t->cr3 = cr3;
}

void init_tss()
{
    init_tss_kernel(paget_dir);
    // init_tss_user();
}