#include <stddef.h>
#include <stdint.h>
#include "tss.h"

struct gdt_entry
{
    uint16_t w1;
    uint16_t w2;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
} __attribute__((packed));

struct gdt_pointer
{
    uint16_t limit;
    uint32_t addr;
} __attribute__((packed));

void init_gdt();

uint8_t install_gdt_kernel_tss(tss *t);
uint32_t install_gdt_user_tss(tss *t);