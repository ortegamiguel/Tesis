#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define T 2

int main()
{
	int *arr,*arr_aux, num, q;
    struct timeval t_ini, t_fin;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }
    //cantidad de hilos
    omp_set_num_threads(T);
    arr_aux = (int *)malloc(num*sizeof(int *));
	//gettimeofday(&t_ini, 0);
    
    #pragma omp parallel shared(arr, num)
    {
    	int gap_actual,j,tmp, gap, l, k, I, gap_ret, i;
    	int tid = omp_get_thread_num();
		//struct timeval t_1, t_2;
        
    	//tamaño del paso inicial
       	gap = (int)(num/2);
       	//controla el tamaño del gap en N/2 por iteracion
    	for (gap_actual = gap, I =0; gap_actual > 0; gap_actual = (int)(gap_actual/2), I++)
    	{            
            //printf("T[%d] --> gap :: %d\n", tid, gap_actual);
            //fflush(0);
            //#pragma omp barrier
            #pragma omp single
            {
                printf("gap_actual[%d] - gap_ret[%d]\n",gap_actual, gap_ret );
                //fflush(0);
                if (gap_ret!=2)
                {
                    printf("antes 11111\n");
                    for(i=0;i<num;i++)
                        printf("a[%d] ",arr[i]);
                    
                    printf("\n");
                    k = 0;
                    for (i = 0; i < gap_ret; i++)
                    {
                        for (j = i; j < num; j = j + (gap_ret/2))
                        {
                            arr[k] = arr_aux[j];
                            k++;            
                        }
                    }
                    printf("despues vuelta \n");
                    for(i=0;i<num;i++)
                        printf("a[%d] ",arr[i]);
                    
                    printf("\n");
                }

                //*transforma a filas!!!*//
                printf("antes \n");
                for(i=0;i<num;i++)
                    printf("aa[%d] ",arr_aux[i]);
                
                printf("\n");
                k = 0;      
                for (i = 0; i < gap_actual; i++)
                {
                    for (j = i; j < num; j = j + gap_actual)
                    {
                        //printf("arr_aux[%d] = arr[%d] --> k :: %d \n", arr_aux[k],arr[j], k);
                        arr_aux[k] = arr[j];
                        k++;            
                    }
                }
                printf("despues \n");
                for(i=0;i<num;i++)
                    printf("aa[%d] ",arr_aux[i]);
                
                printf("\n");
                
            }
		    #pragma omp barrier
			//controla los hilos, trabaja de forma circular
    		/*for (j = tid; j < gap_actual; j = j + T)
    		{
    			//parte en la posicion siguiente de donde esta el hilo
            	for (k = j*gap_ret + 1; k <= j*gap_ret+gap_ret; k++)
            	{
              		//printf("T[%d] :: arr[%d] - arr[%d] - k[%d] - j[%d] - l[%d] - gap_actual[%d]\n", tid, l, l-gap_actual, k, j, l, gap_actual);
                    //fflush(0);
    				if (arr_aux[k] < arr_aux[k-1])
	    			{
	    				//hace la permutacion de los numeros
                        tmp=arr_aux[k];
                        arr_aux[k]=arr_aux[k-1];
                        arr_aux[k-1]=tmp;
               		}
    				else
    				{
                        break;
    				}
    			}        
    		}*/
	    }
    }//end pragma omp parallel
    
    //gettimeofday(&t_fin, 0);
   	//printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);
    //for(q=0;q<num;q++)
        //printf("[%d] ", arr[q]);
	
    free(arr);
    free(arr_aux);
	return 0;
}
