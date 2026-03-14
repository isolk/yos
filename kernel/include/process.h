#include <stddef.h>
#include <stdint-gcc.h>
#include "elf.h"
#include "tss.h"
#include "page.h"
#include "linked_list.h"

#ifndef _PROCESS_
#define _PROCESS_
enum TaskState
{
	running,
	blocked,
	exit
};

typedef struct task_struct
{
	uint32_t pid;
	void *entry;
	page_table *page_dir; // 1024;
	uint8_t state;
	tss *_tss;
	uint32_t sleep_until_tick;
	struct task_struct *next;
	struct task_struct *prev;
} task_struct;

extern task_struct *cur_task; // 指向当前进程

void start();

void process_schedule();

void process_sleep_ms(uint32_t ms);

void process_wake_sleeping(uint32_t current_tick);

void exit_process();

#endif