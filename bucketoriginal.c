//bucket sort generico, crea una cantidad de buckets igual al numero de elementos de entrada
//en donde cada elemento se va al bucket determinado por la siguiente ecuacion
//ptrIndex = (valor*Numero de elementos)/(valor maximo de entrada + 1)
//la complejidad está en como crear esa cantidad de arreglos.
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

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
	int i, j, mem;
    long long int n;
    long long int cociente, dividendo, ptrIndex, size_bucket, higher;
	int *arr;
    struct timeval t_ini, t_fin;
	bucket *buckets;
  	higher = -1; //contendra el valor del mayor elemento
	mem = 1	;
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
	//size_bucket = ((higher - smaller) / T) + 1;

	/*
			Se pide memoria para cada bucket de la estructura,
			se pedira solo un espacio para un elemento,
			en el caso que se necesite de mas memoria se hara
			una re asignacion de memoria segun sea necesaria
	*/
    size_bucket = higher/n;
	for (i = 0;i < n;i++) {
		buckets[i].count = 0;
		buckets[i].stack = (int *)calloc(size_bucket,sizeof(int));
	}

    //printf("N :: %d - Mayor :: %d\n", n, higher);
	/*############ Bloque Paralelo ##############*/
	//omp_set_num_threads(T);
    gettimeofday(&t_ini, 0);
    //cada hilo recorre el arreglo, si el elemento revisado, corresponde a los limites que tiene el bucket,
    //se guarda, en caso contrario sigue revisando hasta terminar de recorrer el arreglo.
    for(i = 0 ; i < n ; i++)
    {
        /*#####################################################################*/
        /* esta parte no funciona, el valor de ptrIndex no sale, debe ser por la declaracion del tipo de dato */
        cociente =  arr[i]*n;
        dividendo = higher+1;
        ptrIndex = cociente/dividendo;
        //printf("Value :: %d - Cociente :: %lli - Dividendo :: %lli - Index :: %lli\n", arr[i], cociente, dividendo, ptrIndex);
        buckets[ptrIndex].count++; //cantidad de elementos por bucket
        buckets[ptrIndex].stack[buckets[ptrIndex].count] = arr[i];
        /*#####################################################################*/
    }

    for (i = 0; i < n; i++) {
        quicksort(buckets[i].stack, 0, buckets[i].count);
    }

    gettimeofday(&t_fin, 0);

    printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);
    //se imprime la cantidad total de elementos partiendo desde el bucket 1, que tiene los elementos menores,
    //y asi sucesivamente, hasta llegar ultimo
    /*
    for (i = 0; i < n; i++) {
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
