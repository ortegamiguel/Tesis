#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int i, j, k, num, *arr, *arr_aux, gap, gap_ret, q, gap_actual;
	scanf("%d", &num);
	getchar();
	arr = malloc(num*sizeof(int *));
	arr_aux = malloc(num*sizeof(int *));
	gap_ret = 2;
	gap = (int)(num/gap_ret);
	
	printf("GAP :: %d\n",gap );

	for ( gap_actual = gap; gap_actual > 0 ; gap_actual = (int)(gap_actual/2))
	{
		k = 0;
		for(q=0; q<num; q++)
	    {
	       	scanf("%d",&arr[q]);
	       	getchar();
	    }
		for (i = 0; i < gap; i++)
		{
			for (j = i; j < num; j = j+gap)
			{
				arr_aux[k] = arr[j];
				k++;			
			}
		}

		for (i = 0; i < num; i++)
		{
			printf("[%d] ", arr[i]);
		}
		printf("\n");
		for (i = 0; i < num; i++)
		{
			printf("[%d] ", arr_aux[i]);
		}


		printf("\n Segunda parte \n");
		k = 0;
		for (i = 0; i < (num/gap_actual); i++)
		{
			for (j = i; j < num; j = j+(num/gap_actual))
			{
				arr[k] = arr_aux[j];
				k++;			
			}
		}

		
		for (i = 0; i < num; i++)
		{
			printf("[%d] ", arr_aux[i]);
		}

		printf("\n");
		
		for (i = 0; i < num; i++)
		{
			printf("[%d] ", arr[i]);
		}
	}
	
	
	return 0;
}