#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void gaussian(int n)
{
	int i;
	long long a,b,c,d, f;
	printf("%d\n",n);
	for (i = 0; i < n; i++)
	{
		a = rand();
		b = rand();
		c = rand();
		d = rand();
		f = (a+b+c+d)/4;
		printf("%lld %lld %lld %lld %lld\n",a,b,c,d, f);
		a=0;b=0;c=0;d=0;f=0;
	}
}

void zero(int n)
{
	int i, a;
	//srand(time(NULL));
	printf("%d\n",n);
	a = rand();
	for (i = 0; i < n; i++)
	{
		printf("%d\n",a);
	}
}

void uniform(int n)
{
	int i, a;
	//srand(time(NULL));
	printf("%d\n",n);
	for (i = 0; i < n; i++)
	{
		a = rand();
		printf("%d\n",a);
	}
}

//	[0,(2^31/p)-1]  - [2^31, (2^32/p)-1] - [...] .... 
void bucket(int n)
{
	long int i,j,a,b,c,d,e,f;
	//srand(time(NULL));
	printf("%d\n",n);
	a = (pow(2,31)/6)-1;
	b = (pow(2,32)/6)-1;
	c = (pow(2,33)/6)-1;
	d = (pow(2,34)/6)-1;
	f = (pow(2,35)/6)-1;
	e = (pow(2,36)/6)-1;

	for (j = 0; j < 5; j++)
	{
		for (i = 0; i < (n/25); i++)
			printf("%ld\n", rand()%a);

		for (i = 0; i < (n/25); i++)
			printf("%ld\n", rand()%a+b);
			
		for (i = 0; i < (n/25); i++)
			printf("%ld\n", rand()%b+c);

		for (i = 0; i < (n/25); i++)
			printf("%ld\n", rand()%c+d);

		for (i = 0; i < (n/25); i++)
			printf("%ld\n", rand()%d+e);
	}

	
}

/*
	[(2i + 1)*(2^31)/p, (2i + 2)*(2^31)/p - 1] -
	[] 
*/
/*El data set es dividido en p bloques,
donde p pertenece a Z+. La distribución staggered
se crea mediante la asignacion de para el bloque i,
donde i < |p/2|, de manera que todos ellos se
encuentran entre (2i +1)((2^31)/p) y (2i +2)((2^31)/p)-1.
Para los bloques donde i > |p/2|, todos los valores
estan entre (2i - p)((2^31)/p) y (2i - p + 1)((2^31)/p) - 1.
Decidimos usar p = 128.
*/
void staggered(int n)
{

}

int main(){
	srand(time(NULL));
	int n;
	scanf("%d",&n);
	//gaussian(n);
	//zero(n);
	uniform(n);
	//bucket(n);
	//staggered(n);
}