#include <interrupt.h>
#include <idt.h>
#include <keyboard.h>
#include <time.h>
#include <gdt.h>
#include <page.h>
#include <ldt.h>
#include <tss.h>
#include <stdarg.h>
#include <string.h>
#include <disk.h>
#include <elf.h>
#include <time.h>

extern struct idt_pointer idt_ptr;

// 比实际情况可能会少1M，不考虑这1M了
void print_mem()
{
    uint16_t *ptr_page_64k = 0x7c00 - 20;
    uint32_t high_mem = *ptr_page_64k * 64 / 1024;

    uint16_t *ptr_page_1k = 0x7c00 - 10;
    uint32_t low_mem = *ptr_page_1k / 1024;

    printf("mem is %d MB", low_mem + high_mem + 1);
}

int _start()
{
    print_mem();

    init_page_all();

    init_gdt();

    init_ldt();

    // 初始化idt
    init_idt();

    // 初始化所有中断
    init_interrupt();

    asm("sti");

    // read_disk(1000, 0x7d000, (uint8_t)128);

    // read_elf(0x7d000); // 用户程序放在21MB处
    // t->eip = 20 * 1024 * 1024 + 0;

    // asm("xchg %bx,%bx");
    // sti();
    // 开启中断，现在开始，中段就会来了。

    // asm("jmpl $0x38,$0");

    for (;;)
    {
        asm("hlt");
    }
    return 0;
}
