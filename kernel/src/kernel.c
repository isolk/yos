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
void keyboard_handler_init();
void default_handler_init();
void page_handler_init();
void syscall_handler_init();
void time_handler_init();

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
    // 打印内存容量
    // 测试代码，先屏蔽中断。
    cli();

    printf("hello,world!  ~!~  ");

    print_mem();

    // 分别设置gdt表的内核数据段和代码段
    init_gdt_code(new_gdt(), 0, 0xFFFFFFFF);
    init_gdt_data(new_gdt(), 0, 0xFFFFFFFF);

    // 创建2个ldt段，分别是任务1和任务2的ldt。他们分别占据gdt的3-4段。 然后所有的ldt段都是指向同一个线性地址，每个ldt段包含有2个段。
    init_gdt_ldt(new_gdt(), new_ldt_begin(), 24);
    init_gdt_ldt(new_gdt(), new_ldt_begin(), 24);

    init_page_all();

    // 创建2个tss段，任务1和2的tss，占据5-6段。
    tss *t = new_tss();
    init_tss1(t, (uint32_t)paget_dir);
    init_gdt_tss(new_gdt(), t, 104);

    tss *t2 = new_tss();
    init_tss2(t2, (uint32_t)paget_dir);
    init_gdt_tss(new_gdt(), t2, 104);

    init_gdt_task(new_gdt(), 0x30);

    // 设置gdtr的内容，然后加载gdtr
    // 但是此时cs ds值还没改
    init_gdt_pointer();
    ll();

    // 到这儿，gdt的东西已经处理完了。 下面我们处理tss段。

    // tss段设置完毕，开始设置中断

    // 初始化中断控制器工作方式
    init_pic();
    init_idtr();
    load_idt(&idt_ptr);
    for (size_t i = 0; i < 32; i++)
    {
        load_idt_entry(i, (uint32_t)handlers[i], 0x08, 0x8e);
    }

    for (size_t i = 32; i < 256; i++)
    {
        load_idt_entry(i, (uint32_t)default_handler_init, 0x08, 0x8e);
    }

    // 缺页处理
    load_idt_entry(14, (uint32_t)page_handler_init, 0x08, 0x8e);

    // 初始化时间的端口设置,设置时间中断
    init_time();
    load_idt_entry(0x28, (uint32_t)time_handler_init, 0x08, 0x8e);

    // 设置键盘中断,以及初始化键盘操作
    load_idt_entry(0x21, (uint32_t)keyboard_handler_init, 0x08, 0x8e);
    init_keyboard();

    load_idt_entry(0x80, (uint32_t)syscall_handler_init, 0x08, 0b11101110);

    read_disk(1000, 0x7d000, (uint8_t)128);

    read_elf(0x7d000); // 用户程序放在21MB处
    t->eip = 20 * 1024 * 1024 + 0;

    lldt();

    // asm("xchg %bx,%bx");
    sti();
    // 开启中断，现在开始，中段就会来了。

    printf("ok,let's go to task 1!");
    // asm("xchg %bx,%bx");
    asm("jmpl $0x38,$0");

    for (;;)
    {
        asm("hlt");
    }
    return 0;
}
