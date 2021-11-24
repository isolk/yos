#include<stddef.h>
#include <stdint-gcc.h>

#ifndef _TSS_
#define _TSS_
typedef struct tss
{
    uint32_t tss;

    uint32_t esp0;
    uint32_t ss0;

    uint32_t esp1;
    uint32_t ss1;

    uint32_t esp2;
    uint32_t ss2;

    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;

    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldt_selector;
    uint32_t io;
    // uint8_t used;
}__attribute__((packed)) tss;

void kernel_init();
void kernel_init_2();
void init_tss();
void install_tss_user(uint32_t cr3, uint32_t eip, uint32_t esp);
extern tss tss_tables[256];
#endif