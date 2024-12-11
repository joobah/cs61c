#include <stdio.h>

int main(void)
{
	printf("\nsizeof table\n");
	printf("type\tsize\n");
	printf("char\t%2d\n", sizeof(char));
	printf("short\t%2d\n", sizeof(short));
	printf("int\t%2d\n", sizeof(int));
	printf("long\t%2d\n", sizeof(long));
	printf("long long\t%2d\n", sizeof(long long));

	return 0;
}
