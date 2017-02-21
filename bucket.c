#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>

#define T 4

int pivot(int *unarray, int izq, int der)
{
    int i;
    int pivote, valor_pivote;
    int aux;

    pivote = izq;
    valor_pivote = unarray[pivote];
    for (i=izq+1; i<=der; i++){
        if (unarray[i] < valor_pivote){
            pivote++;
            aux=unarray[i];
            unarray[i]=unarray[pivote];
            unarray[pivote]=aux;
        }
    }
    aux=unarray[izq];
    unarray[izq]=unarray[pivote];
    unarray[pivote]=aux;
    return pivote;
}

void Quicksort(int *unarray, int izq, int der)
{
     int pivote;
     if(izq < der){
        pivote=pivot(unarray, izq, der);
        Quicksort(unarray, izq, pivote-1);
        Quicksort(unarray, pivote+1, der);
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
		int n, i, j, higher, size_bucket, mem, smaller;
		int *arr;
    struct timeval t_ini, t_fin;
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
		size_bucket = ((higher - smaller) / T) + 1;

		/*
			Se pide memoria para cada bucket de la estructura,
			se pedira solo un espacio para un elemento,
			en el caso que se necesite de mas memoria se hara
			una re asignacion de memoria segun sea necesaria
		*/

		for (i = 0;i < T;i++) {
				buckets[i].count = 0;
				buckets[i].stack = (int *)calloc(mem,sizeof(int));
		}

    //printf("Menor :: %d - Mayor :: %d\n", smaller, higher);
		/*############ Bloque Paralelo ##############*/
		omp_set_num_threads(T);
    gettimeofday(&t_ini, 0);
		#pragma omp parallel shared(arr, buckets)
		{
				int i;
				int tid = omp_get_thread_num();
        //printf("TID :: %d --- Limite Superior :: %d\n", tid, );
				//printf("TID :: %d --- Limite inferior :: %d --- Limite Superior :: %d\n", tid, tid*size_bucket+smaller,tid*size_bucket + smaller+size_bucket);
        //cada hilo recorre el arreglo, si el elemento revisado, corresponde a los limites que tiene el bucket,
        //se guarda, en caso contrario sigue revisando hasta terminar de recorrer el arreglo.
        for(i=0 ; i<n ; i++)
				{
						if( arr[i] >= (tid*size_bucket + smaller) && arr[i] < (tid*size_bucket + smaller + size_bucket) )
						{
								buckets[tid].count++; //cantidad de elementos por bucket
                //se pide un espacio mas cada vez que se encuentra un nuevo elemento que debe entrar en el bucket
                buckets[tid].stack = (int *)realloc(buckets[tid].stack, buckets[tid].count*sizeof(int));
								buckets[tid].stack[buckets[tid].count-1] = arr[i];
						}
				}
        #pragma omp barrier
        Quicksort(buckets[tid].stack, 0, buckets[tid].count);
		}
    gettimeofday(&t_fin, 0);
	  printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);
    //se imprime la cantidad total de elementos partiendo desde el bucket 1, que tiene los elementos menores,
    //y asi sucesivamente, hasta llegar ultimo
		for (i = 0; i < T; i++)
		{
        //este ciclo parte en uno, xq en la ultima iteracion de la busqueda de elementos, se agrega un espacio al bucket,
        //por lo tanto se agrega un cero dentro del bucket que no debe ser considerado.
				for (j = 1; j <= buckets[i].count; j++)
				{
						//printf("%d\n", buckets[i].stack[j]);
				}
		}
    free(arr);
		free(buckets);
		return 0;
}
