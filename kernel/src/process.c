#include "process.h"
#include "disk.h"
#include "elf.h"
#include "pm.h"
#include "page.h"
#include "gdt.h"
#include "idle.h"
#include "string.h"
#define cnew(TYPE) kalloc(sizeof(TYPE))

task_struct *cur_task; // 指向当前进程
task_struct *new_task(task_struct *t)
{
	task_struct *new_ts = cnew(task_struct);
	if (t == NULL)
	{
		new_ts->next = new_ts;
		new_ts->prev = new_ts;
		return new_ts;
	}

	task_struct *t_next = t->next;
	t->next = new_ts;
	new_ts->next = t_next;
	t_next->prev = new_ts;
	new_ts->prev = t;
	return new_ts;
}

task_struct *delete_task(task_struct *t)
{
	task_struct *prev = t->prev;
	prev->next = t->next;
	t->next->prev = prev;
	kfree(t);
	return t->next;
}

void start()
{
	cur_task = new_task(NULL);
	tss *ts = new_tss_kernel((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024, idle);
	cur_task->_tss = ts;
	cur_task->pid = 0;
	cur_task->page_dir = paget_dir;

	uint8_t flag = install_gdt_kernel_tss(ts);
	elf_fh *addr = kalloc(128 * 512);
	read_disk(1000, addr, (uint8_t)128);
	read_elf(addr, 0); // 用户程序放在21MB处
	tss *ts2 = new_tss_user((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024, addr->entry, 22 * 1024 * 1024);
	uint8_t flag2 = install_gdt_user_tss(ts2);

	task_struct *t2 = new_task(cur_task);
	t2->_tss = ts2;
	t2->pid = 1001;
	// t2->page_dir = ;

	// 一开始为idle，调度进init
	process_schedule();
}

uint8_t in_schedule = 0;
void process_schedule()
{
	printf("schedule\n");
	if (in_schedule)
	{
		return;
	}
	in_schedule = 1;
	if (cur_task->state == exit)
	{
		cur_task = delete_task(cur_task);
		jmp_process(cur_task);
	}
	else if (cur_task->next != cur_task)
	{
		cur_task = cur_task->next;
		jmp_process(cur_task);
	}
}

void jmp_process(task_struct *t)
{
	uint8_t flag = 0;
	if (t->pid < 1000)
	{
		flag = install_gdt_kernel_tss(t->_tss);
	}
	else
	{
		flag = install_gdt_user_tss(t->_tss);
	}

	if (flag == 4)
	{
		in_schedule = 0;
		asm("jmp 0x20,0");
	}
	else
	{
		in_schedule = 0;
		asm("jmp 0x28,0");
	}
}

void exit_process()
{
	printf("exit_schedule\n");
	cur_task->state = exit;
	process_schedule();
}

// 给一个task的某虚拟地址，映射一段物理内存，并返回物理内存的起始位置，要求映射为4k的倍数
void *map_task(task_struct *t, void *vaddr, size_t frame)
{
	// 对于一个虚拟地址，高10位是页目录，中间10位是页表，低12位是页内偏移
	// 第一步，首先找到页目录偏移，取高10位，页目录索引
	uint32_t dirIndex = (uint32_t)vaddr >> 22;
	// 第二步，找页表偏移，取中间10位,也就是页表中的第几项
	uint32_t tableIndex = (uint32_t)vaddr << 10 >> 22;

	// | avl(2) | g | 0 | d | a | pcd | pwt | us | rw | p

	uint32_t *dirEntry = &(t->page_dir->entrys[dirIndex]);
	// 表示页表不存在
	if (*dirEntry & 0x1 == 0)
	{
		// 分配的地址就是低12位为0的
		*dirEntry = (uint32_t)kalloc_frame(1) | 0x7;
	}

	// 页表的地址，也就是1024个entry
	page_table *pageTable = *dirEntry & 0xFFFFF000;

	uint32_t *pageEntry = &(pageTable->entrys[tableIndex]);
	if (*pageEntry & 0x1 == 1)
	{
		// 表示这个虚拟地址已经被映射，不能再次被映射了！
		return -1;
	}

	uint32_t p_addr = kalloc_frame(1);
	pageEntry = p_addr | 0x7;
	return p_addr;
}
