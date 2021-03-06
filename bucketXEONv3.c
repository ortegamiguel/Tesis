#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>

#define T 240

__attribute__((target(mic))) void quicksort(int *number, int first, int last)
{
   int i, j, pivot, temp;
   if(first<last){
      pivot=first;
      i=first;
      j=last;
      while(i<j)
      {
         while(number[i] <= number[pivot] && i<last)
            i++;
         while(number[j] > number[pivot])
            j--;
         if(i<j)
         {
            temp=number[i];
            number[i]=number[j];
            number[j]=temp;
         }
      }

      temp=number[pivot];
      number[pivot]=number[j];
      number[j]=temp;
      quicksort(number,first,j-1);
      quicksort(number,j+1,last);

   }
}

//la estructura tendra la cantidad de elementos en ella y un puntero con los elementos
struct _buckets
{
		int count;
		int *stack;
};
typedef struct _buckets bucket;

int main()
{
		int n, i, j, higher, smaller;
		int *arr;


		higher = -1; //contendra el valor del mayor elemento
		smaller = INT_MAX; //contendra el numero del menor elemento

		scanf("%d",&n);
		arr = calloc(n,sizeof(int ));
		//buckets = (bucket *)calloc(T,sizeof(bucket));
		/*
			Lectura y verificacion del elemento mayor y menor
		*/
		for(i=0;i<n;i++)
		{
				scanf("%d", &arr[i]);
				if(arr[i] > higher)
				{
						higher = arr[i];
				}
				else if(arr[i] < smaller)
				{
						smaller = arr[i];
				}
		}
		/*el rango de cada bucket será determinado por la operacion MAYOR / n*/
		/*se tendran tantos buckets como cantidad de hilos definidos*/


		/*
			Se pide memoria para cada bucket de la estructura,
			se pedira solo un espacio para un elemento,
			en el caso que se necesite de mas memoria se hara
			una re asignacion de memoria segun sea necesaria
		*/





    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(n) alloc_if(1) free_if(0))


    #pragma offload target(mic:0) mandatory \
    nocopy(arr:length(n) alloc_if(0) free_if(0))\
    in(n, size_bucket, higher, smaller)
    {
        int mem = 0;
        struct timeval t_ini, t_fin;
        int size_bucket = higher/n;
        int range = ((higher - smaller)/T)+1;
        bucket *buckets;
  	    buckets = (bucket *)calloc(T, sizeof(bucket));
        for (i = 0;i < T;i++) {
    				buckets[i].count = 0;
    				buckets[i].stack = (int *)calloc(size_bucket,sizeof(int));
    		}

        gettimeofday(&t_ini, 0);
        omp_set_num_threads(T);
        #pragma omp parallel shared(arr, buckets)
        {
            int i, cota_maxima, cota_minima;
            int tid = omp_get_thread_num();
            cota_minima = tid*range + smaller;
            cota_maxima = tid*range + smaller + range;
            //cada hilo recorre el arreglo, si el elemento revisado, corresponde a los limites que tiene el bucket,
            //se guarda, en caso contrario sigue revisando hasta terminar de recorrer el arreglo.
            for(i=0 ; i<n ; i++)
            {
                if( arr[i] >= cota_minima && arr[i] < cota_maxima )
                {
                    buckets[tid].count++; //cantidad de elementos por bucket
                    buckets[tid].stack[buckets[tid].count-1] = arr[i];
                }
            }
            #pragma omp barrier
            Quicksort(buckets[tid].stack, 0, buckets[tid].count);
        }
        int k=0;
        for (i = 0; i < T; i++)
    		{
            //este ciclo parte en uno, xq en la ultima iteracion de la busqueda de elementos, se agrega un espacio al bucket,
            //por lo tanto se agrega un cero dentro del bucket que no debe ser considerado.
    				for (j = 1; j <= buckets[i].count; j++)
    				{
    						arr[k] = buckets[i].stack[j];
                k++;
    				}
    		}
        gettimeofday(&t_fin, 0);
        printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);

        //arreglo ordenado
        for (k = 0; k < n; k++) {
          printf("%d ", arr[k] );
        }

    }
    //se imprime la cantidad total de elementos partiendo desde el bucket 1, que tiene los elementos menores,
    //y asi sucesivamente, hasta llegar ultimo

    //free(buckets);
    #pragma offload_transfer target(mic:0) mandatory \
    nocopy(arr:length(n) alloc_if(0) free_if(0))



    //free(arr);
		//free(buckets);
		return 0;
}


/*
export MIC_ENV_PREFIX=PHI
export PHI_KMP_AFFINITY=compact
export PHI_KMP_PLACE_THREADS=60c,4t
export PHI_OMP_NUM_THREADS=240
*/
