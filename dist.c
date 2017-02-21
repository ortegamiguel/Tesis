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
		printf("%lld\n",f);
		a=0;b=0;c=0;d=0;f=0;
	}
}

void zero(int n)
{
	int i, a;
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
	long long a,b;
	int i,j,p, exp;
	
	p = 128;
	exp = 31;
	printf("%d\n",n);
	
	a = 1;
	b = (pow(2,exp)/p)-1;
	for (j = 0; j < p; j++)
	{
		for (i = 0; i < (int)(n/p); i++)
		{
			printf("%lld\n", rand()%a+b);
		}
		a = pow(2,exp)/p;
		exp++;
		b = (pow(2,exp)/p)-1;
	}
}

/*
	[(2i + 1)*(2^31)/p, (2i + 2)*(2^31)/p - 1] -
	[] 
*/
/*El data set es dividido en p bloques,
donde p pertenece a Z+. La distribución staggered
se crea mediante la asignacion de valores para el bloque i,
si i < |p/2|, todos los valores se encontran entre (2i +1)((2^31)/p) y (2i +2)((2^31)/p)-1.
Para los bloques donde i > |p/2|, los valores
seran entre (2i - p)((2^31)/p) y (2i - p + 1)((2^31)/p) - 1.
Decidimos usar p = 128.
*/
void staggered(int n)
{
	int p, i, j;
	long long a,b;
	p = 128;
	printf("%d\n",n);
	for (i = 0; i < p; i++)
	{
		for (j = 0; j < (n/p); j++)
		{
			if (i <= (p/2))
			{
				a = (2i +1)*((pow(2,31))/p);
				b = (2i +2)*((pow(2,31))/p)-1;
				printf("%lld\n",rand()%a+b);
			}else
			{
				a = (2i - p)*((pow(2,31))/p);
				b = (2i - p + 1)*((pow(2,31)/p)) - 1;
				printf("%lld\n",rand()%a+b);
			}
		}
	}
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
