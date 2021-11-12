#include <ldt.h>
#include "gdt.h"

struct ldt_entry ldt_tables[64 * 3];

void init_ldt()
{
    // code
    struct ldt_entry *l = &ldt_tables[0];
    l->w1 = 0xFFFF;
    l->b2 = 0b10011010; //p_dpl(2)_s_type(4)
    l->b3 = 0b11000000; //g_d/b_l_avl_limit(19-16)
    l->b3 |= 0xF;

    // data
    l = &ldt_tables[1];
    l->w1 = 0xFFFF;
    l->b2 = 0b10010010; //p_dpl(2)_s_type(4) 92 1001 0010
    l->b3 = 0b11000000; //g_d/b_l_avl_limit(19-16)
    l->b3 |= 0xF;
}