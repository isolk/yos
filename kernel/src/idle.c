#include "string.h"
#include "tss.h"
#include "page.h"
void idle()
{
	while (1)
	{
		asm("sti");
		asm("hlt");
	}
}