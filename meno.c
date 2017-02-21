#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	int n, *arr, i;
	double menor = 0;

	scanf("%d", &n);
	getchar();
	arr = malloc(n*sizeof(int *));

	for (i = 0; i < n; i++)
	{
		scanf("%d", &arr[i]);
		getchar();
	}

	for (int i = 0; i < n; ++i)
	{
		if (menor < arr[i])
		{
			menor = arr[i];
		}
	}

	printf("%f\n", menor);
	return 0;
}
