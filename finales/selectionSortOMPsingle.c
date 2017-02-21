#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define T 2

int main()
{
	int *arr, *aux_arr, *aux_arr_pos,num, q, min;
	struct timeval t_ini, t_fin;
	struct timeval t_iit, t_fit;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int ));
    aux_arr = malloc(T*sizeof(int ));
    aux_arr_pos = malloc(T*sizeof(int ));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }
    omp_set_num_threads(T);
	gettimeofday(&t_ini, 0);
    
    #pragma omp parallel shared(arr, num, min, aux_arr, aux_arr_pos)
    {
    	int tid = omp_get_thread_num();
    	int i, j, k,tmp, pos;
    	
    	for (i = 0; i < num; i++)
	    {
	    	gettimeofday(&t_iit, 0);
	    	aux_arr[tid] = INT_MAX;
	    	for (j = i + tid; j < num; j= j + T)
	    	{
		   		if (arr[j] < aux_arr[tid])
		   		{
		   			aux_arr[tid] = arr[j];
		   			aux_arr_pos[tid] = j;
		    	}
		    }
		    
		    #pragma omp barrier	
		   	#pragma omp single
		   	{
		   		min = aux_arr[0];
		   		pos = aux_arr_pos[0];

		   		for (k = 1; k < T; k++)
		   		{
		   			if (aux_arr[k] < min)
		   			{
		   				min = aux_arr[k];
		   				pos = aux_arr_pos[k];
		   			}
		   		}
		   		tmp = arr[i];
		   		arr[i] = min;
		   		arr[pos] = tmp

		   		gettimeofday(&t_fit, 0);
			   	printf("Iteracion :: [%d] -- Tiempo :: %f  segundos\n\n", i,(t_fit.tv_sec - t_iit.tv_sec) + (float)(t_fit.tv_usec - t_iit.tv_usec)/1000000.0);
				fflush(0);
		   	}
		   	#pragma omp barrier
	    }
    }

	gettimeofday(&t_fin, 0);
	printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);

	printf("%d\n", num);
    for (q = 0; q < num; q++)
    	printf("%d\n",arr[q] );

	return 0;
}
