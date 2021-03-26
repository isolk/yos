#include<stddef.h>
#include <stdint.h>
void load_idt_entry(size_t int_number,uint32_t addr,uint16_t selector,uint8_t flags);
void init_idtr();
void load_idt(void*);