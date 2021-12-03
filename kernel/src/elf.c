#include <elf.h>
#include <stddef.h>
#include <stdint-gcc.h>
#include <mem.h>
#include "pm.h"

void init_elf(elf *e)
{
	e->file_header = *(elf_fh *)e;
	elf_fh fh = e->file_header;
	elf_ph *ph = e->program_headers;

	elf_ph *addr = NULL;
	for (size_t i = 0; i < fh.phnum; i++)
	{
		addr = fh.phoff + i * fh.phentsize + (uint32_t)e;
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
	for (size_t i = 0; i < fh.phnum; i++)
	{
		void *addr = v_addr;
		if (i > 0)
		{
			addr = v_addr + (ph[i - 1].filesz / ph[i - 1].align + 1) * ph[i].align;
		}
		mem_copy(ph[i].offset + e, addr - 3 * 1024 * 1024 * 1024, ph[i].filesz);
	}
}