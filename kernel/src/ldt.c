#include<ldt.h>

struct ldt_entry ldt_tables[64*3];
int ldt_entry_index = 0;

void init_ldt_null(struct ldt_entry* l)
{
    l->w1 = 0;
    l->w2 = 0;
    l->b1 = 0;
    l->b2 = 0;
    l->b3 = 0; 
    l->b4 = 0;
}

void init_ldt_code(struct ldt_entry* l,uint32_t addr, uint32_t limit)
{
    l->w1 = limit;
    l->w2 = addr;
    l->b1 = addr >> 16;
    l->b2 = 0b11111010;//p_dpl(2)_s_type(4)
    l->b3 = 0b11000000;//g_d/b_l_avl_limit(19-16)
    l->b3 |= (limit << 12) >> 28;
    l->b4 = addr >> 24;
}

void init_ldt_data(struct ldt_entry* l,uint32_t addr, uint32_t limit)
{
    l->w1 = limit;
    l->w2 = addr;
    l->b1 = addr >> 16;
    l->b2 = 0b11110010;//p_dpl(2)_s_type(4) 92 1001 0010
    l->b3 = 0b11000000;//g_d/b_l_avl_limit(19-16)
    l->b3 |= (limit <<12) >> 28;
    l->b4 = addr >> 24;
}

struct ldt_entry *new_ldt_begin()
{

    struct ldt_entry *result = &(ldt_tables[ldt_entry_index*3]);
    init_ldt_null(&(ldt_tables[ldt_entry_index*3]));
    init_ldt_data(&(ldt_tables[ldt_entry_index*3+1]),0,0xFFFFFFFF);
    init_ldt_code(&(ldt_tables[ldt_entry_index*3+2]),0,0xFFFFFFFF);
    ldt_entry_index++;
    return result;
}