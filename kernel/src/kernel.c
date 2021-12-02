#include <interrupt.h>
#include <idt.h>
#include <keyboard.h>
#include <time.h>
#include <gdt.h>
#include <page.h>
#include <ldt.h>
#include <stdarg.h>
#include <string.h>
#include <disk.h>
#include <time.h>
#include <pm.h>
#include "process.h"

extern struct idt_pointer idt_ptr;

int _start()
{
    for (size_t i = 0; i < 2000; i++)
    {
        printf(" ");
    }

    init_mem();

    print_mm();

    init_page_all();

    // init_tss();

    init_gdt();

    init_ldt();

    init_idt();

    init_interrupt();

    asm("cli");

    start();

    for (;;)
    {
        asm("hlt");
    }
    return 0;
}
