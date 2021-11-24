#include <tss.h>
#include "page.h"
#include "string.h"
#include "pm.h"
#include "process.h"

tss tss_tables[256];

void kernel_init()
{
    // asm("sti");
    // process *p = process_manager.head->next->data;
    // set_gdt_kernel_tss(1, p->_tss);
    // asm("call $0x28,$0");

    while (1)
    {
        printf("idle\n");
        asm("hlt");
    }
}

void kernel_init_2()
{
    while (1)
    {
        printf("init22222222222222222222222222222222222222222\n");
        asm("hlt");
    }
}

void init_tss_kernel(uint32_t cr3)
{
    tss *t = &tss_tables[0];
    t->tss = 0;
    // t->eip = kernel_run;
    t->eflags = 0x200;
    t->esp = kalloc(4096) + 4096;

    t->cs = 0x08;
    t->es = 0x10;
    t->ss = 0x10;
    t->ds = 0x10;
    t->fs = 0x10;
    t->gs = 0x10;
    t->ldt_selector = 0x18;
    t->io = 0x8000000;
    t->cr3 = cr3;
}

void install_tss_user(uint32_t cr3, uint32_t eip, uint32_t esp)
{
    tss *t = &tss_tables[1];
    // for (size_t i = 0; i < 256; i++)
    // {
    //     if (!tss_tables[i].used)
    //     {
    //         t = &tss_tables[i];
    //         break;
    //     }
    // }

    t->eip = eip;
    t->esp = esp;

    t->io = 1;
    t->tss = 0;
    t->eflags = 0x200;
    t->ss0 = 0b10000;
    t->esp0 = kalloc(4096) + 4096;

    t->cs = 0b00111;
    t->es = 0b01111;
    t->ss = 0b01111;
    t->ds = 0b01111;
    t->fs = 0b10011;
    t->gs = 0b01111;
    t->ldt_selector = 0x18 | 0x11;
    t->io = 0x8000000;
    t->cr3 = cr3;
}

void init_tss()
{
    // for (size_t i = 0; i < 256; i++)
    // {
    //     tss_tables[i].used = 0;
    // }
    // init_tss_kernel((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024);
}