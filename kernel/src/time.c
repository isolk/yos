#include <time.h>
#include "idt.h"
#include <string.h>
#include <stdint-gcc.h>
#include <io.h>
#include <terminal.h>
#include "process.h"
#include "gdt.h"

#define TIMER_HZ 1024u

static volatile uint32_t timer_ticks = 0;

void init_time()
{
    uint8_t value = 0;

    timer_ticks = 0;

    write_port_b(0x70, 0x8a);
    value = read_port_b(0x71);
    write_port_b(0x70, 0x8a);
    write_port_b(0x71, (value & 0xf0) | 0x06);

    write_port_b(0x70, 0x8b);
    value = read_port_b(0x71);
    write_port_b(0x70, 0x8b);
    write_port_b(0x71, value | 0x40);

    write_port_b(0x70, 0x0c);
    read_port_b(0x71);

    write_port_b(0xa1, 0x00);
}

void time_handler()
{
    write_port_b(0x70, 0x0c);
    read_port_b(0x71);

    timer_ticks++;
    process_wake_sleeping(timer_ticks);

    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);

    // show_time();

    process_schedule();
    // asm("jmpl $0x20,$0");
}

uint32_t timer_get_ticks()
{
    return timer_ticks;
}

uint32_t timer_ms_to_ticks(uint32_t ms)
{
    if (ms == 0)
    {
        return 0;
    }

    return (uint32_t)(((uint64_t)ms * TIMER_HZ + 999u) / 1000u);
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
    write_port_b(0x20, 0x20);
    write_port_b(0xa0, 0x20);
}
