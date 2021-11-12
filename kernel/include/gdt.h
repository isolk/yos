#include<stddef.h>
#include<stdint-gcc.h>

struct gdt_entry
{
    uint16_t w1;
    uint16_t w2;
    uint8_t b1;
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

void init_gdt_data(struct gdt_entry* l,uint32_t addr, uint32_t limit);

void init_gdt_code(struct gdt_entry* l,uint32_t addr, uint32_t limit);

void init_gdt_ldt(struct gdt_entry* g,uint32_t addr, uint32_t limit);

void init_gdt_tss(struct gdt_entry* g,uint32_t addr, uint32_t limit);

void init_gdt_task(struct gdt_entry* g,uint16_t selector);

struct gdt_entry* new_gdt();

void init_gdt();