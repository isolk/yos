#include <string.h>
#include <stdlib.h>
int _start()
{
	int *val = malloc(5000);
	printf("%x . ", val);

	// int *val2 = malloc(6);
	// printf("%x . ", val2);
	// free(val2);

	// int *val3 = malloc(6);
	// printf("%x . ", val3);
	// free(val3);

	// free(val);

	int *val4 = malloc(800);
	printf("%x . ", val4);
	free(val4);

	return 0;
}