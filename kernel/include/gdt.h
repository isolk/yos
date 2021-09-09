#include<stddef.h>
#include<stdint.h>

struct gdt_entry
{
    uint16_t w1;
    uint16_t w2;
    uint16_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
}__attribute__((packed));

struct gdt_pointer
{
    uint16_t limit;
    uint32_t addr;
}__attribute__((packed));

void load_gdt_segment(uint8_t flags);

void Load_gdt_tss();

void load_gdt_ldt();

void load_gdt_call(uint32_t addr,uint8_t params_count);

void ll();

void init_gdt_pointer();

void put_char_c(size_t row,size_t column,uint8_t str);

void init_gdt_ldt(struct gdt_entry* g,uint32_t addr, uint32_t limit);

void init_gdt_tss(struct gdt_entry* g,uint32_t addr, uint32_t limit);

struct gdt_entry* new_gdt();