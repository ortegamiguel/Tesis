#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define T 2

#define ALLOC alloc_if(1) free_if(0)
#define FREE alloc_if(0) free_if(1)
#define REUSE alloc_if(0) free_if(0)

int main(int argc, char const *argv[])
{
	int *arr;
	int num, i, q;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }

    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(num) ALLOC)\

    #pragma offload target(mic:0) mandatory \
    nocopy(arr:length(num) REUSE)\
    in(num)
    {
        int *arr_aux;
        //cantidad de hilos
        omp_set_num_threads(T);
        arr_aux = (int *)_mm_malloc(num*sizeof(int), 64);
        
        #pragma omp parallel shared(arr, arr_aux, num)
        {
            int gap_actual, gap, i, j, k, l, tmp;
            int tid = omp_get_thread_num();
			int go = 1;
            gap = (int)(num/2);

            for (gap_actual = gap; gap_actual > 0; gap_actual = (int)(gap_actual/2))
            {
                #pragma omp single
                {
                    //printf("gap_actual = %d - X = %d\n", gap_actual, (int)(num/gap_actual));
                    //fflush(0);
                    //printf("Hace el arreglo :: arr_aux\n");
                    /*Matriz Fila*/
                    k = 0;
                    for (i = 0; i < gap_actual; i++)
                    {
                        for (j = i; j < num; j = j + gap_actual)
                        {
                            arr_aux[k] = arr[j];
                            k++;
                        }
                    }

                    /*for (i = 0; i < num; i++)
                    {
                        printf("[%d] ", arr_aux[i]);
                    }
                    printf("\n\n");*/
                }
                #pragma omp barrier
                for (j = tid; j < gap_actual; j = j + T)
                {
                    //parte en la posición 1 de la fila, y avanza de uno en uno, hasta que
                    //se recorre la fila que se tiene que ordenar con ese hilo.
                    for (k = j*((int)(num/gap_actual)) + 1;k < j*((int)(num/gap_actual))+(int)(num/gap_actual); k++)
                    {
                        //parte en la posicion de 'k', y retrosede de uno en uno hasta llegar al principio
                        //de la fila que se debe ordenar
						//#pragma omp simd
                        for (l = k;l > j*((int)(num/gap_actual)); l--)
                        {
                            //printf("T[%d] :: arr_aux[%d] - arr_aux[%d] - k[%d] - j[%d] - l[%d] - gap_actual[%d]\n", tid, l, l-1, k, j, l, gap_actual);
                            //fflush(0);
                            if (arr_aux[l] < arr_aux[l-1])
                            {
                                tmp=arr_aux[l];
                                arr_aux[l]=arr_aux[l-1];
                                arr_aux[l-1]=tmp;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }        
                }
                //#pragma omp barrier
                #pragma omp single
                {
                    /*Matriz Original, despues de ordenar, y cambiar a arreglo principal*/
                    /*printf("Antes de ordenar al arreglo Original\n");
                    for (i = 0; i < num; i++)
                    {
                        printf("[%d] ", arr_aux[i]);
                    }
                    printf("\n\n");*/
                    
                    k = 0;
                    for (i = 0; i < (int)(num/gap_actual); i++)
                    {
                        for (j = i; j < num; j = j + (int)(num/gap_actual))
                        {
                            arr[k] = arr_aux[j];
                            k++;
                        }           
                    }
                    /*printf("Despues de ordenar :: arr\n");
                    for (i = 0; i < num; i++)
                    {
                        printf("[%d] ", arr[i]);
                    }
                    printf("\n\n");*/
                }
                #pragma omp barrier
            }
        }
        printf("Arreglo Auxiliar\n");
        for (i = 0; i < num; i++)
        {
            printf("[%d] ", arr_aux[i]);
        }
        /*printf("\n");
        printf("Arreglo Original\n");
        for (i = 0; i < num; i++)
        {
            printf("[%d] ", arr[i]);
        }*/
    }

    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(num) FREE)\
    

    

    //free(arr);
    //free(arr_aux);
	return 0;
}
