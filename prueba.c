#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	int a,b,c,d,f;
	srand(time(NULL));

	a = rand();
		b = rand();
		c = rand();
		d = rand();

		f = (a+b+c+d)/4;
		printf("%d\n", f);
	printf("%d\n",a);
	printf("%d\n",b);
	printf("%d\n",c);
	printf("%d\n",d);
	return 0;
}
