#include <elf.h>
#include <stddef.h>
#include <stdint-gcc.h>
#include <mem.h>
#include "pm.h"

elf main_elf;
void *read_elf(elf_fh *s_addr, void *mem_addr)
{
	main_elf.file_header = *s_addr;

	elf_fh fh = main_elf.file_header;
	elf_ph *ph = main_elf.program_headers;
	elf_ph *addr = NULL;
	for (size_t i = 0; i < fh.phnum; i++)
	{
		addr = fh.phoff + i * fh.phentsize + (uint32_t)s_addr;
		ph[i] = *addr;
	}

	for (size_t i = 0; i < fh.phnum; i++)
	{
		cp_elf(ph + i, (uint32_t)s_addr - 3 * 1024 * 1024 * 1024);
	}

	return s_addr->entry;
}

void cp_elf(elf_ph *ph, void *mem_addr)
{
	// mem_copy(ph->offset + mem_addr, ph->paddr, ph->filesz);
	// 先向内核申请一端物理内存，然后将代码段、数据段复制到对应的物理段内。
	// 然后进行虚拟地址和物理地址的表映射。
	mem_copy(ph->offset + mem_addr, kalloc(ph->filesz), ph->filesz);
}