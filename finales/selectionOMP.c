#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define T 2

int main(int argc, char const *argv[])
{
	int *arr, num, q, min;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }

    omp_set_num_threads(T);

    #pragma omp parallel shared(arr, num, min)
    {
    	int tid = omp_get_thread_num();
    	int i, j, tmp;
    	for (i = 0; i < num; i++)
	    {
	    	#pragma omp single
	    	{
	    		min = i;
	    	}
	    	#pragma omp barrier
	    	for (j = i + tid + 1; j < num; j=j+T)
		   		if (arr[j] < arr[min])
		   			min = j;
		    	
		   	#pragma omp single
		   	{
		   		tmp = arr[i];
		   		arr[i] = arr[min];
		   		arr[min] = tmp;	
		   	}
		   	#pragma omp barrier
	    }
    }

    for (q = 0; q < num; q++)
    	printf("[%d] ",arr[q] );

	return 0;
}