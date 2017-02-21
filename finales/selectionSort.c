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
    	int i, j, k,tmp, min_local, pos;
    	
    	for (i = 0; i < num; i++)
	    {
	    	aux_arr[tid] = INT_MAX;
	    	for (j = i + tid; j < num; j= j + T)
	    	{
		   		if (arr[j] < aux_arr[tid])
		   		{
		   			aux_arr[tid] = min_local;
		   			aux_arr_pos[tid] = j;
		    	}
		    	/*printf("i[%d] - j[%d] - TID[%d] - min_local[%d] \n",i, j, tid, min_local);
	    		fflush(0);*/
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
		   		arr[pos] = tmp;
		   	}
		   	#pragma omp barrier
	    }
    }

	gettimeofday(&t_fin, 0);
	printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);


	return 0;
}
