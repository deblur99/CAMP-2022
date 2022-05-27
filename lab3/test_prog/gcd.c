#include <stdio.h>

int gcd(int a, int b);

void main()
{
	int a = 0x1298;
	int b = 0x09387;
	int res;

	res = gcd(a, b);

	printf("%d\n", res);
}

int gcd(int a, int b)
{
	if (a==b) return a;
	else if (a>b) return gcd(a-b, b);
	else return gcd(b-a, a);
}
