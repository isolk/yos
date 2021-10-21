#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <mem.h>

elf main_elf;
void read_elf(elf_fh *s_addr)
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
		cp_elf(ph + i);
	}

	return;
}

void cp_elf(elf_ph *ph)
{
	cp(ph->offset + 0x7d000, ph->vaddr + 1024 * 1024, ph->filesz);
}