#include <string.h>
#include <stdlib.h>
int _start()
{
	int *val = malloc(sizeof(int));
	*val = 100;
	printf("hello,world,%d", *val);
	free(val);

	int *val2 = malloc(sizeof(int));
	*val2 = 105;
	printf("hello,world,%d,%d", *val, *val2);
	return 0;
}