#include <terminal.h>
#include <string.h>
#include<interrupt.h>
#include<idt.h>
#include<keyboard.h>
#include<time.h>
#include<gdt.h>

extern struct  idt_pointer idt_ptr;
void keyboard_handler_init();
void time_handler_init();

int _start()
{
    // 代码段
    load_gdt_segment(0x98);

    // 数据段
    load_gdt_segment(0x92);

    // put_char的调用门
    load_gdt_call((uint32_t)&put_char,3);

    init_gdt_pointer();

    //asm volatile ("xchg %bx, %bx");

    ll();

    asm volatile ("xchg %bx, %bx");
    
    put_char_c(0,0,'h');

    asm volatile ("xchg %bx, %bx");

    // put_char_c(0,1,'!');

    //asm volatile ("xchg %bx, %bx");

    // // 初始化中断控制器工作方式
    // init_pic();
    // // 设置idtr的地址
    // init_idtr();
    // // 真正加载idtr
    // load_idt(&idt_ptr);

    // // 加载键盘中断处理程序
    // load_idt_entry(0x21,(uint32_t)keyboard_handler_init,0x08,0x8e);
    // load_idt_entry(0x28,(uint32_t)time_handler_init,0x08,0x8e);

    // // 打开键盘中断
    // init_keyboard();
    // init_time();
    
    for(;;){}
    return 0;
}