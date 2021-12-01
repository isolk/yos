#include "process.h"
#include "disk.h"
#include "elf.h"
#include "pm.h"
#include "page.h"
#include "gdt.h"
#include "idle.h"
#include "string.h"

linked_node *cur_process; // 指向当前进程
void start()
{
	cur_process = new_list();
	process *p = kalloc(sizeof(process));
	cur_process->data = p;
	tss *ts = new_tss_kernel((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024, idle);
	p->_tss = ts;
	p->pid = 0;

	uint8_t flag = install_gdt_kernel_tss(ts);
	elf_fh *addr = kalloc(128 * 512);
	read_disk(1000, addr, (uint8_t)128);
	read_elf(addr, 0); // 用户程序放在21MB处
	tss *ts2 = new_tss_user((uint32_t)paget_dir - 3 * 1024 * 1024 * 1024, addr->entry, 22 * 1024 * 1024);
	uint8_t flag2 = install_gdt_user_tss(ts2);
	p = kalloc(sizeof(process));
	p->_tss = ts2;
	p->pid = 1001;
	append_data(cur_process, p);

	cur_process = cur_process->next;
	asm("jmp 0x20,0");
}

uint8_t in_schedule = 0;
void process_schedule()
{
	if (in_schedule)
	{
		return;
	}
	in_schedule = 1;
	// 每次调度时，直接简单的取下一个进程就可以了。如果就1个，什么都不做。
	if (cur_process->next == cur_process)
	{
		in_schedule = 0;
		return;
	}
	cur_process = cur_process->next;
	process *p = cur_process->data;
	if (p->state == -1)
	{
		linked_node *t = cur_process->next;
		delete_node(cur_process);
		cur_process = t;
	}
	// 跳转process
	jmp_process(cur_process->data);
}

void jmp_process(process *p)
{
	uint8_t flag = 0;
	if (p->pid < 1000)
	{
		flag = install_gdt_kernel_tss(p->_tss);
	}
	else
	{
		flag = install_gdt_user_tss(p->_tss);
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
	in_schedule = 1;
	process *p = cur_process->data;
	p->state = 1;
	in_schedule = 0;
	process_schedule();
}