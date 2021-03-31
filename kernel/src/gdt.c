#include<gdt.h>
#include<idt.h>

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


struct gdt_entry gdt_tables[1000];
struct gdt_pointer gdt_ptr;
int entry_index = 1;

void init_gdt_pointer()
{
    gdt_ptr.limit = 1000*8-1;
    gdt_ptr.addr = (uint32_t)&gdt_tables;
}

// 全局段只允许0开始，4GB，0特权
void load_gdt_segment(uint8_t flags)
{
    gdt_tables[entry_index].w1 = 0xFFFF;
    gdt_tables[entry_index].w2 = 0x0000;
    gdt_tables[entry_index].b1 = 0x00;
    gdt_tables[entry_index].b2 = flags;
    gdt_tables[entry_index].b3 = 0xCF;
    gdt_tables[entry_index].b4 = 0x00;
    entry_index++;
}

void ll()
{
    lgdtr(&gdt_ptr);
}

void load_gdt_call(uint32_t addr,uint8_t params_count)
{
    gdt_tables[entry_index].w1 = addr & 0xFFFF;// 0-1
    gdt_tables[entry_index].w2 =  0x0008;// 00000000_00001_0_00 G,0 02
    gdt_tables[entry_index].b1 = params_count & 0x10;  // 5位可用
    gdt_tables[entry_index].b2 = 0x8C; //10001100 
    gdt_tables[entry_index].b3 = (addr >> 16) & 0xFF; // 2
    gdt_tables[entry_index].b4 = (addr >> 24) & 0xFF; // 3
    entry_index++; 
}