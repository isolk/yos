#include <mm.h>
uint32_t begin_brk;
uint32_t end_brk;
// 将当前的堆地址设置到这个位置
void brk(uint32_t addr)
{
	if (addr > end_brk)
	{
		end_brk = addr;
	}
}