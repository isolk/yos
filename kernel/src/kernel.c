#include<interrupt.h>
#include<idt.h>
#include<keyboard.h>
#include<time.h>
#include<gdt.h>
#include<page.h>
#include<ldt.h>

extern struct  idt_pointer idt_ptr;
void keyboard_handler_init();
void time_handler_init();
void put_char_init();

int _start()
{
    sti();
    // set gdt in 0x0->8*256
    print_string("hello",5);
    // InitPageDir();

    // InitPageTable();

    // init_page();
    
    // 代码段
    load_gdt_segment(0x98);

    // 数据段
    load_gdt_segment(0x92);

    // ldt1 2
    init_gdt_ldt(new_gdt(),new_ldt_begin(),32*3-1);
    // init data code null
    init_gdt_ldt(new_gdt(),new_ldt_begin(),32*3-1);

    // tss1 2
    init_gdt_tss(new_gdt,);
    init_gdt_tss();

    // put_char的调用门
    // load_gdt_call((uint32_t)&put_char_init,3);

    init_gdt_pointer();


    ll();

    // 初始化中断控制器工作方式
    init_pic();
    // 设置idtr的地址
    init_idtr();
    // 真正加载idtr
    load_idt(&idt_ptr);


    for (size_t i = 0; i < 256; i++)
    {
        load_idt_entry(i,(uint32_t)time_handler_init,0x08,0x8e);
    }

    // 加载键盘中断处理程序
    load_idt_entry(0x21,(uint32_t)keyboard_handler_init,0x08,0x8e);
    load_idt_entry(0x28,(uint32_t)time_handler_init,0x08,0x8e);
    

    init_time();

    // 打开键盘中断
    init_keyboard();

    init_task();

    // go to fisrt task

    
    for(;;){
        asm("hlt");
    }
    return 0;
}

void task1()
{
    print_string("task A",6);
}

void task2()
{
    print_string("task B",6);
}