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

	uint8_t flag = install_gdt_kernel_tss(ts);
	elf_fh *addr = kalloc(128 * 512);
	read_disk(1000, addr, (uint8_t)128);
	read_elf(addr, 0); // 用户程序放在21MB处
	tss *ts2 = new_tss_user((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024, addr->entry, 22 * 1024 * 1024);
	uint8_t flag2 = install_gdt_user_tss(ts2);

	task_struct *t2 = new_task(cur_task);
	t2->_tss = ts2;
	t2->pid = 1001;

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
