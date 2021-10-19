#include<stddef.h>
#include<stdint.h>

// 读取elf文件并加载到内存中，s_addr：elf在内存中起始地址，d_addr:要放置的目标内存地址
void read_elf(uint32_t s_addr,uint32_t d_addr);