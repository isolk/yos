#include "process.h"
#include "disk.h"
#include "elf.h"
#include "pm.h"
#include "page.h"
#include "gdt.h"

linked_list process_manager;

uint8_t _pid = 1;
void load_entry(void *entry)
{
	process *p = kalloc(sizeof(process));
	push_data(&process_manager, p);
	p->entry = entry;
	p->page_dir = paget_dir;
	p->pid = _pid + 1;

	tss *t = kalloc(sizeof(tss));
	t->tss = 0;
	t->eip = entry;
	t->eflags = 0x200;
	t->esp = kalloc(4096) + 4096;

	t->cs = 0x08;
	t->es = 0x10;
	t->ss = 0x10;
	t->ds = 0x10;
	t->fs = 0x10;
	t->gs = 0x10;
	t->ldt_selector = 0x18;
	t->io = 0x8000000;
	t->cr3 = (uint32_t)paget_dir - 3 * 1024 * 1024 * 1024;
	p->_tss = t;
}

linked_node *cur = NULL;
uint8_t index = 0;
void start()
{
	cur = process_manager.head;
	process *p = cur->data;
	set_gdt_kernel_tss(0, p->_tss);
	asm("jmpl $0x20,$0");
}

void process_schedule()
{
	// 先找到当前繁忙的tss
	// 	然后设置不繁忙的tss地址
	// 跳转到不繁忙的tss
	if (cur->next != NULL)
	{
		cur = cur->next;
	}
	else if (cur != process_manager.head)
	{
		cur = process_manager.head;
	}
	else
	{
		return;
	}

	process *p = cur->data;
	index += 1;
	index = index % 2;
	set_gdt_kernel_tss(index, p->_tss);
	if (index == 0)
	{
		asm("jmpl $0x20,$0");
	}
	else
	{
		asm("jmpl $0x28,$0");
	}
}