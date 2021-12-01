#include <tss.h>
#include "page.h"
#include "string.h"
#include "pm.h"
#include "process.h"

tss *new_tss_kernel(uint32_t page_dir, uint32_t eip)
{
    tss *t = kalloc(sizeof(tss));
    t->tss = 0;
    t->eip = eip;
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
    t->cr3 = page_dir;
    return t;
}

tss *new_tss_user(uint32_t page_dir, uint32_t eip, uint32_t esp)
{
    tss *t = kalloc(sizeof(tss));
    t->tss = 0;
    t->eip = eip;
    t->eflags = 0x200;
    t->esp = kalloc(4096) + 4096;

    t->ss0 = 0b10000;
    t->esp0 = kalloc(4096) + 4096;

    t->cs = 0b00111;
    t->es = 0b01111;
    t->ss = 0b01111;
    t->ds = 0b01111;
    t->fs = 0b01111;
    t->gs = 0b01111;
    t->ldt_selector = 0x18 | 0x11;
    t->io = 0x8000000;
    t->cr3 = page_dir;
    return t;
}