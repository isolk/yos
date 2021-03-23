#include <stdint.h>
#include <stddef.h>
void put_char(size_t row,size_t column,uint8_t str);
void clear();
void print_char(uint8_t str);
void print_string(char* str,size_t len);
void set_cur();

#define ROW 2
#define WIDTH 10