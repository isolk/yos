#include <terminal.h>
void put_char(size_t row, size_t column, uint8_t str)
{
    uint8_t *tr = (uint8_t *)0xb8000 + 3 * 1024 * 1024 * 1024;
    tr = tr + row * WIDTH * 2 + column * 2;
    *tr = str;
    set_cur(row * WIDTH + column);
}

uint8_t get_char(size_t row, size_t column)
{
    uint8_t *tr = (uint8_t *)0xb8000 + 3 * 1024 * 1024 * 1024;
    tr = tr + row * WIDTH * 2 + column * 2;
    return *tr;
}