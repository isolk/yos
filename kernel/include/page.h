#include <stdint-gcc.h>
#include <stddef.h>
#ifndef _PAGE_
#define _PAGE_
typedef struct page_entry
{

	// ptr（20）           | avl(2) | g | 0 | d | a | pcd | pwt | us | rw | p
	// ptr：页表的物理地址前20位
	// avl：程序使用
	// g：全局
	// d：脏，表示该页写过数据
	// a：表示是否被访问过
	// pcd、pwt：忽略吧
	// us：是否允许所有特权访问，如果为0，表示3特权不能访问
	// rw：是否可写，1表示可读也可写。
	// p：该页表或页是否存在
	uint32_t data; // ptr（20）           | avl(2) | g | 0 | d | a | pcd | pwt | us | rw | p
} page_entry;

typedef struct page_table
{
	uint32_t entrys[1024];
} page_table;

void init_page_all();
extern struct page_entry *paget_dir;
#endif