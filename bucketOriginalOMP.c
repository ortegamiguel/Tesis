#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>

#define T 2

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
		int i, j, mem;
		int *arr;
    long long int n;
    long long int size_bucket, higher;
    struct timeval t_ini, t_fin;
		bucket *buckets;
		higher = -1; //contendra el valor del mayor elemento
		mem = 0	;
		scanf("%lli",&n);
		arr = calloc(n,sizeof(int ));
		buckets = (bucket *)calloc(n,sizeof(bucket));
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
		}
		/*el rango de cada bucket será determinado por la operacion MAYOR / n*/
		/*se tendran tantos buckets como cantidad de hilos definidos*/
		size_bucket = higher/n;

		/*
			Se pide memoria para cada bucket de la estructura,
			se pedira solo un espacio para un elemento,
			en el caso que se necesite de mas memoria se hara
			una re asignacion de memoria segun sea necesaria
		*/

		for (i = 0;i < T;i++) {
				buckets[i].count = 0;
				buckets[i].stack = (int *)calloc(size_bucket,sizeof(int));
		}

    //printf("Menor :: %d - Mayor :: %d\n", smaller, higher);
		/*############ Bloque Paralelo ##############*/
		omp_set_num_threads(T);
    //gettimeofday(&t_ini, 0);
		#pragma omp parallel shared(arr, buckets, higher, n) //6000816000
		{
				int i;
        long long int cociente, dividendo, ptrIndex;
				int tid = omp_get_thread_num();
        //printf("TID :: %d --- Limite Superior :: %d\n", tid, );
				//printf("TID :: %d --- Limite inferior :: %d --- Limite Superior :: %d\n", tid, tid*size_bucket+smaller,tid*size_bucket + smaller+size_bucket);
        //cada hilo recorre el arreglo, si el elemento revisado, corresponde a los limites que tiene el bucket,
        //se guarda, en caso contrario sigue revisando hasta terminar de recorrer el arreglo.
        for(i = tid ; i < n ; i = i + T)
				{
            cociente =  arr[i]*n;
            dividendo = higher+1;
            ptrIndex = cociente/dividendo;
            //sascascsd
                buckets[ptrIndex].count++; //cantidad de elementos por bucket
                //se pide un espacio mas cada vez que se encuentra un nuevo elemento que debe entrar en el bucket
                buckets[ptrIndex].stack[buckets[ptrIndex].count-1] = arr[i];

				}
        #pragma omp barrier

        Quicksort(buckets[tid].stack, 0, buckets[tid].count);
		}
    //gettimeofday(&t_fin, 0);
	  //printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);
    //se imprime la cantidad total de elementos partiendo desde el bucket 1, que tiene los elementos menores,
    //y asi sucesivamente, hasta llegar ultimo
    /*for (i = 0; i < n; i++) {
        printf("Bucket[%d] :: ",i );
        for (j = 1; j <= buckets[i].count; j++) {
            printf("%d ", buckets[i].stack[j]);
        }
        printf("\n");
    }*/
    free(arr);
		free(buckets);
		return 0;
}
