#include <stdint-gcc.h>
#include <stddef.h>
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

// 内核：1-11M
// 0-4k 页目录
// 内核的页表

struct page_entry *paget_dir = (struct page_entry *)(0); // 设置地址为0
// 内核的页表
struct page_entry *paget_table = (struct page_entry *)(4 * K); // 设置地址为4k

// 在此代码执行之时，内核被loader加载到物理地址的1-11M处。将内核所占用的物理地址(0-11M）映射到虚拟地址中，其他地址设置为页不存在。
void InitPageDir()
{
    //0-1024，每一个dir项包含1024个地址映射，也就是1024*4KB=4MB。 内核放在第0-11M，也就是第1-3个direntry处
    for (size_t i = 0; i < 1 * K; i++)
    {
        paget_dir[i].data = 0;
    }

    uint32_t ptr = (uint32_t)paget_table;
    ptr = ptr & 0xFFFFF000; // 清零低12位
    paget_dir[0].data = ptr | 0x3;
    paget_dir[1].data = (ptr + 4 * K) | 0x3;
    paget_dir[2].data = (ptr + 8 * K) | 0x3;
}

void InitPageTable()
{
    //对于内核空间，进行相同映射。也就是对于地址小于12MB的线性地址，全部映射成相同的物理地址.
    for (size_t i = 0; i < 3 * K; i++)
    {
        uint32_t ptr = (uint32_t)(i << 12);
        paget_table[i].data = ptr | 0x3;
    }
}
