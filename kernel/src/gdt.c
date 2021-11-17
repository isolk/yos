#include "gdt.h"
#include "idt.h"
#include "tss.h"
#include "ldt.h"

#define lgdt(addr) asm( \
    "lgdt (%0);"        \
    :                   \
    : "r"(addr))

struct gdt_entry gdt_tables[256];
struct gdt_pointer gdt_ptr;

/*
    0 null

    1 code-kernel
    2 data-kernel

    3 ldt-user

    4 tss-kernel
    5 tss-user

    6 task_gate - kernel
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

    // gdt[4] = tss dpl = 0 内核进程的tss
    g = &gdt_tables[4];
    tss *tss_addr = &tss_tables[0];
    g->w1 = 104;
    g->w2 = tss_addr;
    g->b1 = (uint32_t)tss_addr >> 16;
    g->b2 = 0b10001001; //p_dpl(2)_s_type(4)
    g->b3 = 0b01000000; //g_d_l_avl_limit(19-16)
    g->b4 = (uint32_t)tss_addr >> 24;

    // gdt[5] = tss dpl = 3 用户进程的tss
    g = &gdt_tables[5];
    tss_addr = &tss_tables[1];
    g->w1 = 104;
    g->w2 = tss_addr;
    g->b1 = (uint32_t)tss_addr >> 16;
    g->b2 = 0b11101001; //p_dpl(2)_s_type(4)
    g->b3 = 0b01000000; //g_d_l_avl_limit(19-16)
    g->b4 = (uint32_t)tss_addr >> 24;

    // gdt[6] = task_gate dpl = 0 任务门
    g = &gdt_tables[6];
    g->w1 = 0;
    g->w2 = 0x20; // 指向 gdt[4]
    g->b1 = 0;
    g->b2 = 0b10000101; //p_dpl(2)_s_type(4)
    g->b3 = 0;
    g->b4 = 0;

    // 设置gdtr的属性
    gdt_ptr.limit = 7 * 8 - 1;
    gdt_ptr.addr = (uint32_t)&gdt_tables;

    // 设置gdrt寄存器
    lgdt(&gdt_ptr);
}