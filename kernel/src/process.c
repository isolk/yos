#include "process.h"
#include "disk.h"
#include "elf.h"
#include "pm.h"
#include "tss.h"

void exec_file(uint32_t start_sector, uint8_t sector)
{
	// kfree(addr);

	// read_disk to mem
	void *cache_addr = kalloc(sector * 512);
	read_disk(start_sector, cache_addr, (uint8_t)128);

	// load bin to mem
	void *bin_addr = kalloc(sector * 512);
	void *entry = read_elf(cache_addr, bin_addr);

	// create tss
	// install_tss_user(, entry, )
	// when timer hit,chage gdt->tss
}
