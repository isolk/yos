
#include "page.h"
#include "pm.h"
// ADDR(20)|AVL(3)|G|0(PAT)|D|A|PCD|PWT|US|RW|P
// p 页存在 1
// rw 可读/可写 1
// us 用户管理，1：允许所有特权。 0 不允许特权3
// pwt 缓存 0
// pcd 缓存 0
// a 是否被访问过 0
// d 脏位，是否被写过 0
// pat 复杂分页，0
// g 是否全局 0
// avl 自定义 0
// adr_000100000011

#define K 1024
#define K4 4096

page_table *paget_dir = 0;   // 1
page_table *paget_table = 0; // 1024

void init_page_all()
{
    init_page_dir();
    init_page_table();
    // init_page((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024); // 要换成物理地址给寄存器
}

void init_page_dir()
{
    paget_dir = kalloc_frame(1);
    paget_table = kalloc_frame(1024);
    // 映射0-3G -> 0-3G
    for (size_t i = 0; i < 768; i++)
    {
        uint32_t ptr = (uint32_t)paget_table - 3 * 1024 * 1024 * 1024;
        paget_dir->entrys[i] = (ptr + i * K4) | 0x7;
    }

    // 映射3-4G -> 0-1G
    for (size_t i = 768; i < K; i++)
    {
        uint32_t ptr = (uint32_t)paget_table + (i - 768) * K4 - 3 * 1024 * 1024 * 1024;
        paget_dir->entrys[i] = ptr | 0x7;
    }
}

void init_page_table()
{
    uint32_t total = 0;
    for (size_t i = 0; i < 1024; i++)
    {
        for (size_t j = 0; j < 1024; j++)
        {
            uint32_t ptr = (uint32_t)(total << 12) | 0x7;
            paget_table[i].entrys[j] = ptr;
            total++;
        }
    }
}