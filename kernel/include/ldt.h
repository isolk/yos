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

void init_ldt_null(struct ldt_entry* l);
void init_ldt_data(struct ldt_entry* l,uint32_t addr, uint32_t limit);
void init_ldt_code(struct ldt_entry* l,uint32_t addr, uint32_t limit);
struct ldt_entry *new_ldt_begin();
void init_k_ldt();
extern struct ldt_entry ldt_tables[64 * 3];