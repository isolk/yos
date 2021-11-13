#include <string.h>
#include <interrupt.h>
#include <stdint-gcc.h>
#include "idt.h"
#define PIC_1_CTRL 0x20
#define PIC_1_DATA 0x21
#define PIC_2_CTRL 0xA0
#define PIC_2_DATA 0xA1

void keyboard_handler_wrap();
void default_handler_wrap();
void page_handler_wrap();
void syscall_handler_wrap();
void time_handler_wrap();

void init_pic()
{
    // begin
    write_port_b(PIC_1_CTRL, 0x11);
    write_port_b(PIC_1_DATA, 0x20);
    write_port_b(PIC_1_DATA, 1 << 2);
    write_port_b(PIC_1_DATA, 0x01);

    write_port_b(PIC_2_CTRL, 0x11);
    write_port_b(PIC_2_DATA, 0x28);
    write_port_b(PIC_2_DATA, 0x01);
    write_port_b(PIC_2_DATA, 0x01);

    write_port_b(0x21, 0xff);
    write_port_b(0xa1, 0xff);
}

void init_interrupt()
{
    // 设置默认的，后面逐步完善
    for (size_t i = 0; i < 256; i++)
    {
        load_idt_entry(i, (uint32_t)default_handler_wrap, 0x08, 0x8e);
    }

    // 初始化pic工作方式
    init_pic();

    // 缺页处理
    load_idt_entry(14, (uint32_t)page_handler_wrap, 0x08, 0x8e);

    // 设置键盘中断,以及初始化键盘操作
    init_keyboard();
    load_idt_entry(0x21, (uint32_t)keyboard_handler_wrap, 0x08, 0x8e);

    // 初始化时间的端口设置,设置时间中断
    init_time();
    load_idt_entry(0x28, (uint32_t)time_handler_wrap, 0x08, 0x8e);

    // 系统中断
    load_idt_entry(0x80, (uint32_t)syscall_handler_wrap, 0x08, 0b11101110);
}