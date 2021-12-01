#include "string.h"
#include "tss.h"
#include "page.h"
void idle()
{
	asm("xchg %bx,%bx");
	while (1)
	{
		printf("idle\n");
		asm("hlt");
	}
}