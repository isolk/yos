#include <string.h>
#include <stdlib.h>
#include <ystd.h>
int _start()
{
	for (int i = 0; i < 10; i++)
	{
		printf("i=%d\n", i);
		sleep(1000);
	}
	yexit(0);
	return 0;
}