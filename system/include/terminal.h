#include <stdint.h>
#include <stddef.h>
void put_char(size_t row,size_t column,uint8_t str);
void clear();
void print_char(uint8_t str);
void print_string(char* str,size_t len);
void set_cur(uint16_t postion);
void outb(uint16_t port,uint8_t value);
uint8_t inb(uint16_t port);
uint8_t get_char(size_t row,size_t column);

#define ROW 25
#define WIDTH 80