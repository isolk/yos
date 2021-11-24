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

void init_gdt();

void set_gdt_kernel_tss(uint8_t index,uint32_t t);