#include<stddef.h>
#include<stdint-gcc.h>

typedef struct ldt_entry
{
    uint16_t w1;
    uint16_t w2;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
}__attribute__((packed)) ldt;

extern struct ldt_entry ldt_tables[64 * 3];

void init_ldt();