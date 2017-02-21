#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>

#define T 2

void quicksort(int *number,int first,int last){
   int i, j, pivot, temp;

   if(first<last){
      pivot=first;
      i=first;
      j=last;

      while(i<j){
         while(number[i] <= number[pivot] && i<last)
            i++;
         while(number[j] > number[pivot])
            j--;
         if(i<j){
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
		int n, i, j, higher, size_bucket, mem, smaller, range;
		int *arr;
    	struct timeval t_ini, t_fin;
		struct timeval t_iniq, t_finq;
		bucket *buckets;
		higher = -1; //contendra el valor del mayor elemento
		smaller = INT_MAX; //contendra el numero del menor elemento
		mem = 0	;
		scanf("%d",&n);
		arr = calloc(n,sizeof(int ));
		buckets = (bucket *)calloc(T,sizeof(bucket));
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
    	range = ((higher - smaller)/T)+1;
    	//size_bucket = higher/n;
    	size_bucket = sqrt(n);
    	//printf("higher :: %d -- n :: %d --- size bucket :: %d \n", higher, n, size_bucket);
    	for ( i = 0; i < T ; i++ )
    	{
			buckets[i].count = 0;
			buckets[i].stack = (int *)calloc(1000*size_bucket,sizeof(int));
		}

		//printf("higher %d\n", higher);
    	/*############ Bloque Paralelo ##############*/

    	omp_set_num_threads(T);
    	gettimeofday(&t_ini, 0);
		#pragma omp parallel shared(arr, buckets)
		{
			int i, cota_minima, cota_maxima;
			int tid = omp_get_thread_num();
        	//cada hilo recorre el arreglo, si el elemento revisado, corresponde a los limites que tiene el bucket,
        	//se guarda, en caso contrario sigue revisando hasta terminar de recorrer el arreglo.
        	cota_minima = tid*range + smaller;
        	cota_maxima = tid*range + smaller + range;
        	for(i = 0 ; i < n ; i++)
			{
				if(arr[i] >= cota_minima && arr[i] < cota_maxima)
				{
					buckets[tid].count++; //cantidad de elementos por bucket
        			buckets[tid].stack[buckets[tid].count-1] = arr[i];
				}
			}
        	#pragma omp barrier
        	gettimeofday(&t_fin, 0);
	  		printf("Tiempo creacion Bucket :: %f  segundos\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);
	  		fflush(0);	
        	gettimeofday(&t_iniq, 0);	
        	quicksort(buckets[tid].stack, 0, buckets[tid].count);
        	#pragma omp barrier
			gettimeofday(&t_finq, 0);
			printf("Tiempo en ordenar :: %f segundos\n", (t_finq.tv_sec - t_iniq.tv_sec) + (float)(t_finq.tv_usec - t_iniq.tv_usec)/1000000.0);
		}
    /*############ Fin Bloque Paralelo ##############*/
    			
		//se imprime la cantidad total de elementos partiendo desde el bucket 1, que tiene los elementos menores,
    //y asi sucesivamente, hasta llegar ultimo
		/*for (i = 0; i < T; i++)
		{
        //este ciclo parte en uno, xq en la ultima iteracion de la busqueda de elementos, se agrega un espacio al bucket,
        //por lo tanto se agrega un cero dentro del bucket que no debe ser considerado.
        printf("iteracion[%d]\n",i );
        for (j = 1; j <= buckets[i].count; j++)
				{
						printf("%d\n", buckets[i].stack[j]);
				}
		}*/
    	free(arr);
		free(buckets);
		return 0;
}
