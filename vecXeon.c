#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sched.h>

#define T 240
#define ALLOC alloc_if(1) free_if(0)
#define FREE alloc_if(0) free_if(1)
#define REUSE alloc_if(0) free_if(0)

int main()
{
	int *arr, num, q;
   	struct timeval t_ini, t_fin;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }

    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(num) ALLOC)

    #pragma offload target(mic:0) mandatory \
    nocopy(arr:length(num) REUSE)\
    in(num)\
	inout(t_ini)\
	inout(t_fin)
    {
		gettimeofday(&t_ini, 0);
		omp_set_num_threads(T);
        int i;
		#pragma omp parallel shared(arr, num)
        {         
			int tid = omp_get_thread_num();
            int gap_actual,j,tmp, i, gap, l, k, I, gap_ret;
            int *arr_aux;  
			struct timeval t_1, t_2;
            //memoria para el array auxiliar
            arr_aux = (int *)_mm_malloc(num*sizeof(int), 64);
            /*Asignación de core*/
            cpu_set_t mascara;
   			CPU_ZERO (&mascara);//'limpia' la variable mascara (la coloca en 0)
            CPU_SET ((int)(tid/4), &mascara);//con 'mascara' se indica el numero del thread en donde se ejecutara el thread actual.
            //A continuacion se establece que el thread actual (el primer parametro '0' indica el thread actual) se ejecute en el core 'mascara'-esimo. 'len' solo indica el largo de la variable 'mascara'. 
            if (sched_setaffinity(0, sizeof(cpu_set_t), &mascara) < 0)
               printf("\nError :: No se pudo hacer la asignacion de un thread a un core\n");
            /*########################################################################################*/      
           	//tamaño del paso inicial
            gap_ret = 2;
            gap = (int)(num/gap_ret);
            //controla el tamaño del gap en N/2 por iteracion
            for (gap_actual = gap, I=0, gap_ret=2; gap_actual > 0; gap_actual = (int)(gap_actual/2), I++, gap_ret*2)
            {           
                //printf("T[%d] --> gap :: %d\n", tid, gap_actual);
                //fflush(0);
                #pragma omp barrier
                if (gap_ret!=2)
                {
                    k = 0;
                    for (i = 0; i < gap_ret; i++)
                        for (j = i; j < num; j = j+gap_ret)
                        {
                            arr[k] = arr_aux[j];
                            k++;            
                        }

					for(i=0;i<num;i++)
						printf("[%d] ",arr[i]);
                }
                k = 0;      
                for (i = 0; i < gap_actual; i++)
                    for (j = i; j < num; j = j + gap_actual)
                    {
                        arr_aux[k] = arr[j];
                        k++;            
                    }

				for(i=0;i<num;i++)
					printf("[%d] ",arr_aux[i]);

                printf("\n");
				/*#pragma omp master
				{
					gettimeofday(&t_1, 0);
				}*/
                //controla los hilos, trabaja de forma circular
                for (j = tid; j < gap_actual; j = j + T)
                {
                    //parte en la posicion siguiente de donde esta el hilo
                    for (k = j + gap_ret; k < num; k = k + gap_ret)
                    {
                        //retrocede comparando, para hacer el ordenamiento
                        for (l = k; l - gap_ret >= 0; l--)
                        {
                            //printf("T[%d] :: arr[%d] - arr[%d] - k[%d] - j[%d] - l[%d] - gap_actual[%d]\n", tid, l, l-gap_actual, k, j, l, gap_actual);
                            //fflush(0);
                            if (arr[l] < arr[l-1])
                            {
                                //hace la permutacion de los enteros
                                tmp=arr[l];
                                arr[l]=arr[l-1];
                                arr[l-1]=tmp;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }        
                }
                #pragma omp barrier
				

                /*#pragma omp master
				{
					gettimeofday(&t_2, 0);
   					printf("\nTiempo iteracion %d (gap_actual=%d) = %f segundos\n", I, gap_actual, (t_2.tv_sec -  t_1.tv_sec)+ (float)(t_2.tv_usec - t_1.tv_usec)/(1000000.0));
				}*/
            }
        }//end pragma omp parallel
		gettimeofday(&t_fin, 0);
   		printf("\nTiempo :: %f  segundos\n", (t_fin.tv_sec -  t_ini.tv_sec)+ (float)(t_fin.tv_usec - t_ini.tv_usec)/(1000000.0));
		for(i=0;i<num;i++)
			printf("[%d] ", arr[i]);
    } // end pragma offload

	//printf("Tiempo :: %f  segundos\n", (t_fin.tv_sec -  t_ini.tv_sec)+ (float)(t_fin.tv_usec - t_ini.tv_usec)/(1000000.0));


    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(num) FREE)
 
    free(arr);
	return 0;
}
