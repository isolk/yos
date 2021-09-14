#include<gdt.h>
#include<idt.h>

struct gdt_entry gdt_tables[256];
struct gdt_pointer gdt_ptr;
int entry_index = 1;

void init_gdt_pointer()
{
    gdt_ptr.limit = 256*8-1;
    gdt_ptr.addr = (uint32_t)&gdt_tables;
}

// // 全局段只允许0开始，4GB，0特权
// void load_gdt_segment(uint8_t flags)
// {
//     gdt_tables[entry_index].w1 = 0xFFFF;
//     gdt_tables[entry_index].w2 = 0x0000;
//     gdt_tables[entry_index].b1 = 0x00;
//     gdt_tables[entry_index].b2 = flags;
//     gdt_tables[entry_index].b3 = 0xCF;
//     gdt_tables[entry_index].b4 = 0x00;
//     entry_index++;
// }

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

struct gdt_entry* new_gdt()
{
    return &(gdt_tables[entry_index++]);
}


void init_gdt_data(struct gdt_entry* l,uint32_t addr, uint32_t limit)
{
    l->w1 = limit;
    l->w2 = addr;
    l->b1 = addr >> 16;
    l->b2 = 0b10010010;//p_dpl(2)_s_type(4) 92 1001 0010
    l->b3 = 0b11000000;//g_d/b_l_avl_limit(19-16)
    l->b3 |= (limit <<12) >> 28;
    l->b4 = addr >> 24;
}


void init_gdt_code(struct gdt_entry* g,uint32_t addr, uint32_t limit)
{
    g->w1 = limit;
    g->w2 = addr;
    g->b1 = addr >> 16;
    g->b2 = 0b10011010;//p_dpl(2)_s_type(4) 9a  1010
    g->b3 = 0b11000000;//g_d_l_avl_limit(19-16)
    g->b3 |= (limit << 12) >> 28;
    g->b4 = addr >> 24;
}

void init_gdt_ldt(struct gdt_entry* g,uint32_t addr, uint32_t limit)
{
    g->w1 = limit;
    g->w2 = addr;
    g->b1 = addr >> 16;
    g->b2 = 0b11100010;//p_dpl(2)_s_type(4)
    g->b3 = 0b01000000;//g_d_l_avl_limit(19-16)
    g->b3 |= (limit << 12) >> 28;
    g->b4 = addr >> 24;
}

void init_gdt_tss(struct gdt_entry* g,uint32_t addr,uint32_t limit)
{
    g->w1 = limit;
    g->w2 = addr;
    g->b1 = addr >> 16;
    g->b2 = 0b11101001;//p_dpl(2)_s_type(4)
    g->b3 = 0b01000000;//g_d_l_avl_limit(19-16)
    g->b3 |= (limit << 12) >> 28;
    g->b4 = addr >> 24;
}

void init_gdt_task(struct gdt_entry* g,uint16_t selector)
{
    g->w1 = 0;
    g->w2 = selector;
    g->b1 = 0;
    g->b2 = 0b10000101;//p_dpl(2)_s_type(4)
    g->b3 = 0;
    g->b4 = 0;
}