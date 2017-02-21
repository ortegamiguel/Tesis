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
		
	}


	return 0;
}