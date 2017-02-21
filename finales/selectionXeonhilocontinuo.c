#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define T 240

#define ALLOC alloc_if(1) free_if(0)
#define FREE alloc_if(0) free_if(1)
#define REUSE alloc_if(0) free_if(0)

int main()
{
	int *arr, *aux_arr, *aux_arr_pos,num, q, min;
	int paso_hilo, resto, aux_num;
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

    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(num) ALLOC)\
    in(aux_arr:length(T) ALLOC)\
    in(aux_arr_pos:length(T) ALLOC)

    #pragma offload target(mic:0) mandatory \
    nocopy(arr:length(num) REUSE)\
    nocopy(aux_arr:length(T) REUSE)\
    nocopy(aux_arr_pos:length(T) REUSE)\
	in(num, min, paso_hilo, resto, aux_num)\
	inout(t_ini)\
	inout(t_fin)
	{
		gettimeofday(&t_ini, 0);
		omp_set_num_threads(T);
		#pragma omp parallel shared(arr, num, min, aux_arr, aux_arr_pos, paso_hilo, resto)
	    {
	    	int tid = omp_get_thread_num();
	    	int i, j, k,tmp, min_local, pos;
	    	int ini, fin;
	    	for (i = 0; i < num; i++)
		    {
		    	//min_local = INT_MAX;
		    	aux_arr[tid] = INT_MAX;
		    	ini = i + tid * paso_hilo;
		    	if (tid == T-1 || i == num-1)
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
				    	//printf("i[%d] - j[%d] - TID[%d] - min_local[%d] \n",i, j, tid, min_local);
			    		//fflush(0);
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
			   		
			   		aux_num--;
			   		paso_hilo = aux_num/T;
	    			resto = aux_num%T;
			   	}
			   	#pragma omp barrier
		    }
	    }
	    gettimeofday(&t_fin, 0);
		printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);

	}
	return 0;
}