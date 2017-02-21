#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define T 240

int main()
{
	int *arr, *aux_arr, *aux_arr_pos,num, q, min;
	int paso_hilo, aux_num;
	struct timeval t_ini, t_fin;
	
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int ));
    aux_arr = malloc(T*sizeof(int ));
    aux_arr_pos = malloc(T*sizeof(int ));
    for(q=0; q<num; q++)
    {
       	scanf("%d", &arr[q]);
       	getchar();
    }
    aux_num = num;
    paso_hilo = aux_num/T;

    omp_set_num_threads(T);
	gettimeofday(&t_ini, 0);
    
    #pragma omp parallel shared(arr, num, min, aux_arr, aux_arr_pos, paso_hilo)
    {
    	int tid = omp_get_thread_num();
    	int i, j, k,tmp, pos;
    	int ini, fin;

	    for (i = 0; i < num; i++)
		{
		   	aux_arr[tid] = INT_MAX;
		   	ini = i + tid * paso_hilo;
		   	
		   	if (paso_hilo <= 0)
		   		ini = i;

	    	if (tid == T-1 || i == num-1 || aux_num-1 == tid || tid > aux_num)
	    		fin = num;
	    	else
	    		fin = i + (tid * paso_hilo) + paso_hilo;
		    		
	    	for (j = ini; j < fin; j++)
		    {
				if (arr[j] < aux_arr[tid])
				{
					aux_arr[tid] = arr[j];
		   			aux_arr_pos[tid] = j;
			    }
			    /*printf("i[%d] - j[%d] - TID[%d] - ini[%d] - fin[%d] - aux_arr[%d]\n",i, j, tid, ini, fin, aux_arr[tid]);
		    	fflush(0);*/
			}	
		    	
		    	
		    #pragma omp barrier	
		   	#pragma omp single
		   	{
		   		/*printf("antes de buscar\n");
		   		int q;
	    		for (q = 0; q < T; q++)
				{
					printf("[%d] ", aux_arr[q]);
				}
				printf("\n");*/
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
			   	
			   	aux_num--;
			   	paso_hilo = aux_num/T;

	    		/*int q;
	    		for (q = 0; q < num; q++)
				{
					printf("[%d] ", arr[q]);
				}
				printf("\n");*/
			}
			#pragma omp barrier
		}
	}
    gettimeofday(&t_fin, 0);
	printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);

	for (q = 0; q < num; q++)
	{
		printf("%d\n", arr[q]);
	}
	

	return 0;
}
