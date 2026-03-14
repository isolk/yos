#include <elf.h>
#include <stddef.h>
#include <stdint-gcc.h>
#include <mem.h>
#include "pm.h"
#include "memlayout.h"

void init_elf(elf *e)
{
	e->file_header = *(elf_fh *)e;
	elf_fh fh = e->file_header;
	elf_ph *ph = e->program_headers;
	uint8_t *raw = (uint8_t *)e;

	elf_ph *addr = NULL;
	for (size_t i = 0; i < fh.phnum; i++)
	{
		addr = (elf_ph *)(raw + fh.phoff + i * fh.phentsize);
		ph[i] = *addr;
	}
}

uint32_t get_elf_psize(elf *e)
{
	uint32_t total = 0;
	elf_fh fh = e->file_header;
	elf_ph *ph = e->program_headers;
	for (size_t i = 0; i < fh.phnum; i++)
	{
		elf_ph p = ph[i];
		total += (p.filesz / p.align + 1) * p.align;
	}
	return total;
}

uint32_t get_elf_vm_start(elf *e)
{
	return e->program_headers[0].vaddr;
}

void cp_elf_ph(elf *e, void *v_addr)
{
	elf_fh fh = e->file_header;
	elf_ph *ph = e->program_headers;
	uint8_t *raw = (uint8_t *)e;
	uint8_t *dest = (uint8_t *)v_addr;
	for (size_t i = 0; i < fh.phnum; i++)
	{
		uint8_t *addr = dest;
		if (i > 0)
		{
			addr = dest + (ph[i - 1].filesz / ph[i - 1].align + 1) * ph[i - 1].align;
		}
		mem_copy(raw + ph[i].offset, addr - KERNEL_BASE, ph[i].filesz);
	}
}