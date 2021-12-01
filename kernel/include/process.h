#include <stddef.h>
#include <stdint-gcc.h>
#include "elf.h"
#include "tss.h"
#include "linked_list.h"

#ifndef _PROCESS_
#define _PROCESS_
typedef struct process
{
	uint32_t pid;
	void *entry;
	void *page_dir;
	uint8_t state;
	tss *_tss;
} process;

extern linked_node *cur_process; // 指向当前进程

void start();

void process_schedule();

void exit_process();

#endif