#include <stddef.h>
#include <stdint-gcc.h>
#include "elf.h"
#include "tss.h"
#include "linked_list.h"

#ifndef _PROCESS_
#define _PROCESS_
typedef struct process
{
	uint8_t pid;
	void *entry;
	void *page_dir;
	tss *_tss;
} process;

extern linked_list process_manager;

// 加载内核进程，传入函数地址。
void load_entry(void *entry);

void load_elf(elf *e);

void start();

void process_schedule();

#endif