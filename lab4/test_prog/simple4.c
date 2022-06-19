#include <stdio.h>

int foo(int a);
int main()

{
	printf("%d\n", foo(10));
}

int foo(int a)
{
	int result;
	if (a == 1) return 1;
	result = a + foo(a-1);
	return result;
}

