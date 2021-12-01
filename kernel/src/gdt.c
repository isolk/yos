#include "gdt.h"
#include "idt.h"
#include "tss.h"
#include "ldt.h"

#define lgdt(addr) asm( \
    "lgdt [%0];"        \
    :                   \
    : "r"(addr))

struct gdt_entry gdt_tables[10];
struct gdt_pointer gdt_ptr;

/*
    0 null
    1 code-kernel
    2 data-kernel
    3 ldt-user
*/

void init_gdt()
{
    // gdt[0] = null 无效段

    // gdt[1] = code dpl = 0 内核代码段
    struct gdt_entry *g = &gdt_tables[1];
    g->w1 = 0xFFFF;
    g->b2 = 0b10011010; //p_dpl(2)_s_type(4) 9a  1010
    g->b3 = 0b11000000; //g_d_l_avl_limit(19-16)
    g->b3 |= 0xF;

    // gdt[2] = data dpl = 0 内核数据段
    g = &gdt_tables[2];
    gdt_tables[1].w1 = 0xFFFFFFFF;
    g->w1 = 0xFFFF;
    g->b2 = 0b10010010; //p_dpl(2)_s_type(4) 92 1001 0010
    g->b3 = 0b11000000; //g_d/b_l_avl_limit(19-16)
    g->b3 |= 0xF;

    // gdt[3] = ldt dpl = 3 用户态ldt段
    g = &gdt_tables[3];
    ldt *ldt_addr = &ldt_tables[0];
    g->w1 = 16;
    g->w2 = ldt_addr;
    g->b1 = (uint32_t)ldt_addr >> 16;
    g->b2 = 0b11100010; //p_dpl(2)_s_type(4)
    g->b3 = 0b01000000; //g_d_l_avl_limit(19-16)
    g->b4 = (uint32_t)ldt_addr >> 24;
    // 设置gdtr的属性
    gdt_ptr.limit = 7 * 8 - 1;
    gdt_ptr.addr = (uint32_t)&gdt_tables;

    // 设置gdrt寄存器
    lgdt(&gdt_ptr);
}

uint8_t last_index = 4;
uint8_t install_gdt_kernel_tss(tss *t)
{
    if (last_index == 4)
    {
        last_index = 5;
    }
    else
    {
        last_index = 4;
    }

    struct gdt_entry *g = &gdt_tables[last_index];
    g->w1 = 104;
    g->w2 = t;
    g->b1 = (uint32_t)t >> 16;
    g->b2 = 0b10001001; //p_dpl(2)_s_type(4)
    g->b3 = 0b01000000; //g_d_l_avl_limit(19-16)
    g->b4 = (uint32_t)t >> 24;

    // 0:0 1:0x08 2:0x10 3:0x18 4:0x20
    return last_index;
}

uint32_t install_gdt_user_tss(tss *t)
{
    if (last_index == 4)
    {
        last_index = 5;
    }
    else
    {
        last_index = 4;
    }

    struct gdt_entry *g = &gdt_tables[last_index];
    g->w1 = 104;
    g->w2 = t;
    g->b1 = (uint32_t)t >> 16;
    g->b2 = 0b11101001; //p_dpl(2)_s_type(4)
    g->b3 = 0b01000000; //g_d_l_avl_limit(19-16)
    g->b4 = (uint32_t)t >> 24;
    return last_index;
}