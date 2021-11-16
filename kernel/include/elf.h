#include <stddef.h>
#include <stdint-gcc.h>

// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format
typedef struct elf_fh
{
	// 0x7f454c46
	uint32_t magic;
	// 1=32,2=64
	uint8_t cpu_type;
	// 1 or 2 to signify little or big endianness
	uint8_t data;
	// = 1
	uint8_t version1;
	// often = 0
	uint8_t osabi;
	uint64_t unused;
	// 0x02 =	ET_EXEC
	uint16_t obj_type;
	// 0x03 =	x86
	uint16_t machine;
	// = 1
	uint32_t version2;

	uint32_t entry;
	uint32_t phoff; // program header table addr
	uint32_t shoff; // section header table addr
	uint32_t flags;
	uint16_t ehsize;	// this file header size
	uint16_t phentsize; // Contains the size of a program header table entry.
	uint16_t phnum;		// Contains the number of entries in the program header table.
	uint16_t shentsize; // Contains the size of a section header table entry.
	uint16_t shnum;		// Contains the number of entries in the section header table.
	uint16_t shstrndx;	// Contains index of the section header table entry that contains the section names.
} __attribute__((packed)) elf_fh;

typedef struct elf_ph
{
	uint32_t type;	 // 1 = Loadable
	uint32_t offset; // Offset of the segment in the file image.
	uint32_t vaddr;	 // Virtual address of the segment in memory.
	uint32_t paddr;	 // On systems where physical address is relevant, reserved for segment's physical address.
	uint32_t filesz; // Size in bytes of the segment in the file image. May be 0.
	uint32_t memsz;	 // Size in bytes of the segment in memory. May be 0.
	uint32_t flags;	 // Segment-dependent flags (position for 32-bit structure).
	uint32_t align;	 // 0 and 1 specify no alignment. Otherwise should be a positive, integral power of 2, with p_vaddr equating p_offset modulus p_align.

} __attribute__((packed)) elf_ph;

typedef struct elf_sh
{
	uint32_t sh_name;	   // An offset to a string in the .shstrtab section that represents the name of this section.
	uint32_t sh_type;	   // Identifies the type of this header.
	uint32_t sh_flags;	   // Identifies the attributes of the section.
	uint32_t sh_addr;	   // Virtual address of the section in memory, for sections that are loaded.
	uint32_t sh_offset;	   // Offset of the section in the file image.
	uint32_t sh_size;	   // Size in bytes of the section in the file image. May be 0.
	uint32_t sh_link;	   // Contains the section index of an associated section. This field is used for several purposes, depending on the type of section.
	uint32_t sh_info;	   // Contains extra information about the section. This field is used for several purposes, depending on the type of section.
	uint32_t sh_addralign; // Contains the required alignment of the section. This field must be a power of two.
	uint32_t sh_entsize;   // Contains the size, in bytes, of each entry, for sections that contain fixed-size entries. Otherwise, this field contains zero.

} __attribute__((packed)) elf_sh;

typedef struct elf
{
	elf_fh file_header;
	elf_ph program_headers[6];
	elf_sh section_headers[6];
} __attribute__((packed)) elf;

// 读取elf文件并加载到内存中
void *read_elf(elf_fh *s_addr, void *mem_addr);