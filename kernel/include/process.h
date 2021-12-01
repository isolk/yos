#include <stddef.h>
#include <stdint-gcc.h>
#include "elf.h"
#include "tss.h"
#include "linked_list.h"

#ifndef _PROCESS_
#define _PROCESS_
enum TaskState
{
	running,
	exit
};

typedef struct task_struct
{
	uint32_t pid;
	void *entry;
	void *page_dir;
	uint8_t state;
	tss *_tss;
	struct task_struct *next;
	struct task_struct *prev;
} task_struct;

extern task_struct *cur_task; // 指向当前进程

void start();

void process_schedule();

void exit_process();

#endif