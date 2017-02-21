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
        #pragma omp parallel shared(arr, num)
        {         
			int tid = omp_get_thread_num();  
			cpu_set_t mascara;
   			struct timeval t_1, t_2;

            CPU_ZERO (&mascara);//'limpia' la variable mascara (la coloca en 0)
            CPU_SET ((int)(tid/4), &mascara);//con 'mascara' se indica el numero del thread en donde se ejecutara el thread actual.
            //CPU_SET (0, &mascara);
        
            //A continuacion se establece que el thread actual (el primer parametro '0' indica el thread actual) se ejecute en el core 'mascara'-esimo. 'len' solo indica el largo de la variable 'mascara'. 
            if (sched_setaffinity(0, sizeof(cpu_set_t), &mascara) < 0)
               printf("\nError :: No se pudo hacer la asignacion de un thread a un core\n");

            int gap_actual,j,tmp, gap, l, k, I;
           
           	//tamaño del paso inicial
            gap = (int)(num/2);
            //controla el tamaño del gap en N/2 por iteracion
            for (gap_actual = gap, I=0; gap_actual > 0; gap_actual = (int)(gap_actual/2), I++)
            {           
               // printf("T[%d] --> gap :: %d\n", tid, gap_actual);
                //fflush(0);		
                #pragma omp barrier
				#pragma omp master
				{
					gettimeofday(&t_1, 0);
				}
                //controla los hilos, trabaja de forma circular
                for (j = tid; j < gap_actual; j = j + T)
                {
                    //parte en la posicion siguiente de donde esta el hilo
                    for (k = j + gap_actual; k < num; k = k + gap_actual)
                    {
                        //retrocede comparando, para hacer el ordenamiento
                        for (l = k; l - gap_actual >= 0; l = l - gap_actual)
                        {
                            //printf("T[%d] :: arr[%d] - arr[%d] - k[%d] - j[%d] - l[%d] - gap_actual[%d]\n", tid, l, l-gap_actual, k, j, l, gap_actual);
                            //fflush(0);
                            if (arr[l] < arr[l-gap_actual])
                            {
                                //hace la permutacion de los numeros
                                tmp=arr[l];
                                arr[l]=arr[l-gap_actual];
                                arr[l-gap_actual]=tmp;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }        
                }
                #pragma omp barrier
				#pragma omp master
				{
					gettimeofday(&t_2, 0);
   					printf("\nTiempo iteracion %d (gap_actual=%d) = %f segundos\n", I, gap_actual, (t_2.tv_sec -  t_1.tv_sec)+ (float)(t_2.tv_usec - t_1.tv_usec)/(1000000.0));
				}
            }
        }//end pragma omp parallel
		gettimeofday(&t_fin, 0);
   		printf("\nTiempo :: %f  segundos\n", (t_fin.tv_sec -  t_ini.tv_sec)+ (float)(t_fin.tv_usec - t_ini.tv_usec)/(1000000.0));
    } // end pragma offload

	//printf("Tiempo :: %f  segundos\n", (t_fin.tv_sec -  t_ini.tv_sec)+ (float)(t_fin.tv_usec - t_ini.tv_usec)/(1000000.0));


    |#pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(num) FREE)
 
    free(arr);
	return 0;
}
