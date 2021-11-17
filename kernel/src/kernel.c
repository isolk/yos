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
#include <pm.h>

extern struct idt_pointer idt_ptr;

int _start()
{
    init_mem();
    // print_mm();

    init_page_all();

    init_tss();

    init_gdt();

    init_ldt();

    init_idt();

    init_interrupt();

    asm("cli");

    // void *addr = kalloc(128 * 512);
    // read_disk(1000, addr, (uint8_t)128);

    // read_elf(addr); // 用户程序放在21MB处
    // // t->eip = 20 * 1024 * 1024 + 0;

    // sti();
    // 开启中断，现在开始，中段就会来了。

    // asm("xchg %bx,%bx");
    asm("jmpl $0x30,$0");

    for (;;)
    {
        asm("hlt");
    }
    return 0;
}
