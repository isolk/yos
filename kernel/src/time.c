#include <time.h>
#include "idt.h"
#include <string.h>
#include <stdint-gcc.h>
int cur_task = 0;
void init_time()
{
    write_port_b(0x70, 0x8b);
    write_port_b(0x71, 0b10110); // 0001_0110

    write_port_b(0x70, 0x0c);
    read_port_b(0x71);

    write_port_b(0xa1, 0x00);
}

void time_handler()
{
    write_port_b(0x70, 0x0c);
    read_port_b(0x71);

    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);

    show_time();

    // print_char('*');

    // 判断当前任务号，使用jmp切换
    // if (cur_task == 0)
    // {
    //     cur_task = 1;
    //     asm("jmp $0x30,$0");
    // }
    // else
    // {
    //     cur_task = 0;
    //     // print_char('#');
    //     asm("jmp $0x28,$0");
    // }
}

void show_time()
{
    write_port_b(0x70, 0x80);
    uint8_t sec = read_port_b(0x71);

    write_port_b(0x70, 0x82);
    uint8_t min = read_port_b(0x71);

    write_port_b(0x70, 0x84);
    uint8_t hour = read_port_b(0x71);

    write_port_b(0x70, 0x87);
    uint8_t day = read_port_b(0x71);

    write_port_b(0x70, 0x88);
    uint8_t month = read_port_b(0x71);

    write_port_b(0x70, 0x89);
    uint8_t year = read_port_b(0x71);

    write_port_b(0x70, 0x0c);
    read_port_b(0x71);

    printf("%d-%d-%d %d:%d:%d\n", year, month, day, hour, min, sec);
}

void default_handler()
{
    write_port_b(0x70, 0x0c);
    read_port_b(0x71);

    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
    print_char('*');
}

#define is_present(err_code) (err_code & 0x01)
#define is_write(err_code) (err_code & 0x02)
#define is_user(err_code) (err_code & 0x04)
#define is_struction(err_code) (err_code & 0x10)
void page_handler(uint32_t cr2, uint32_t err_code)
{
    int u = is_user(err_code);
    int w = is_write(err_code);
    int s = is_struction(err_code);
    int p = is_present(err_code);
    if (u)
    {
        printf("errcode=%x,cr2=%x", err_code, cr2);
    }
    printf("errcode=%x,cr2=%x", err_code, cr2);
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
