#include <stdint-gcc.h>
#include <stddef.h>
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
struct page_entry
{
    uint32_t data;
};

#define K 1024
#define K4 4096

// 内核：1-11M
// 0-4k 页目录
// 内核的页表

struct page_entry *paget_dir = 0;
// 内核的页表
struct page_entry *paget_table = 0;

void init_page_all()
{
    init_page_dir();
    init_page_table();
    // init_page(paget_dir);
}

// 在此代码执行之时，内核被loader加载到物理地址的1-11M处。将内核所占用的物理地址(0-11M）映射到虚拟地址中，其他地址设置为页不存在。
void init_page_dir()
{
    paget_dir = kalloc_frame(1);
    paget_table = kalloc_frame(1024); // TODO: 请求时要请求多个页。否则只分配一个页，不满足需求，下面table映射的内容就有问题，现在没问题是因为对应的物理地址没有被访问。
    // 0-1024，每一个dir项包含1024个地址映射，也就是1024*4KB=4MB。 内核放在第0-11M，也就是第1-3个direntry处
    for (size_t i = 768; i < 1 * K; i++)
    {
        uint32_t ptr = (uint32_t)paget_table;
        ptr = ptr & 0xFFFFF000; // 清零低12位
        uint32_t k = i - 768;
        paget_dir[i].data = (ptr + k * K4) | 0x7;
    }
}

void init_page_table()
{
    //对于内核空间，进行相同映射。也就是对于地址小于12MB的线性地址，全部映射成相同的物理地址.
    for (size_t i = 0; i < 32 * K4; i++)
    {
        uint32_t ptr = (uint32_t)(i << 12);
        paget_table[i].data = ptr | 0x7;
    }
}
