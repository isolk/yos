#include<stddef.h>
#include<stdint.h>

void load_gdt_segment(uint8_t flags);

void Load_gdt_tss();

void load_gdt_ldt();

void load_gdt_call(uint32_t addr,uint8_t params_count);

void ll();

void init_gdt_pointer();

void put_char_c(size_t row,size_t column,uint8_t str);