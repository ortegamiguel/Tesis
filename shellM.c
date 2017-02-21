#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define T 2   

int main()
{
	int *arr, num, q;
    time_t start, end;
    

  
	scanf("%d",&num);
	printf("\n");
	//printf("%d\n",num );
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
        scanf("%d",&arr[q]);
        getchar();
    }
    

    omp_set_num_threads(T);

    start = time(NULL);
    
    #pragma omp parallel shared(arr, num)
    {
    	int gap_actual,j,tmp, gap, l, k;
    	int tid = omp_get_thread_num();
    	//tamaño del paso inicial
        gap = (int)(num/2);
        //controla el tamaño del gap en N/2 por iteracion
    	for (gap_actual = gap; gap_actual > 0; gap_actual = (int)(gap_actual/2))
    	{
            //if (tid == 0)
            printf("gap = %d\n", gap_actual);
            #pragma omp barrier
            //controla los hilos, trabaja de forma circular
    		for (j = tid; j < num && tid < gap_actual; j=j+T)
    		{
                //parte en la posicion siguiente de donde esta el hilo
                for (k=j+gap_actual; k<num; k+=gap_actual)
                {
                    //retrocede comparando, para hacer el ordenamiento
    				for (l=k; l-gap_actual>=0; l-=gap_actual)
    				{
                        //printf("T[%d] :: arr[%d] - arr[%d]\n", tid, l, l-gap_actual);
    					if (arr[l] < arr[l-gap_actual])
	    				{
                            //hace la permutacion de los numeros
	    					tmp=arr[l];
			                arr[l]=arr[l-gap_actual];
			                arr[l-gap_actual]=tmp;
    					}
    					else
    						break;
    				}
                }        
    		}
            //#pragma omp barrier
            //if (tid == 0)
                //printf("--------\n");
            //#pragma omp barrier
    	}
    }//end pragma omp parallel
    end = time(NULL);
    printf("Tiempo :: %f  \n",  difftime(end, start));
    //printf("%.16g milisegundos\n", secs * 1000.0);

    //printf("\t**** Shell Sorting ****\n");
    /*
    for(q=0; q<num; q++)
        printf("%d\t",arr[q]);
*/
	return 0;
}


