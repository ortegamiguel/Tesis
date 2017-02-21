#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>

#define T 240

__attribute__((target(mic))) void quicksort(int *number,int first,int last){
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
    int i, j, size_bucket, quantity, smaller, range;
    int *arr;
    long long int n, higher;
    
      
    
    higher = -1; //contendra el valor del mayor elemento
    smaller = INT_MAX; //contendra el numero del menor elemento
    //mem = 0 ;
    scanf("%lli",&n);
    arr = calloc(n,sizeof(int ));
    
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
    
      
    

    /*############ Bloque Paralelo ##############*/
    
    #pragma offload_transfer target(mic:0) mandatory \
    in(arr:length(n) alloc_if(1) free_if(0))

    #pragma offload target(mic:0) mandatory \
    nocopy(arr:length(n) alloc_if(0) free_if(0))\
    in(n, higher, range)
    {
        int i;
        struct timeval t_ini, t_fin;
        long long int cociente, dividendo, ptrIndex;
        bucket *buckets;
        range = higher/n;
        buckets = (bucket *)calloc(n,sizeof(bucket));

        for ( i = 0; i < n ; i++ )
        {
            buckets[i].count = 0;
            buckets[i].stack = (int *)calloc(range,sizeof(int));
        }
        
        omp_set_num_threads(T);
        gettimeofday(&t_ini, 0);
    
        #pragma omp parallel shared(arr, buckets)
        {
            int i;
            int tid = omp_get_thread_num();
            for(i = tid ; i < n ; i = i + T)
            {
                cociente =  arr[i]*n;
                dividendo = higher+1;
                ptrIndex = cociente/dividendo;
                buckets[ptrIndex].count++;
                buckets[ptrIndex].stack[buckets[ptrIndex].count] = arr[i];
            }

            #pragma omp barrier
            for (i = 0; i < n; i++)
            {
                quicksort(buckets[i].stack, 0, buckets[i].count);
            }
        }
        
        gettimeofday(&t_fin, 0);
        printf("Tiempo :: %f  segundos\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);
        /*############ Fin Bloque Paralelo ##############*/
              
        //se imprime la cantidad total de elementos partiendo desde el bucket 1, que tiene los elementos menores,
          //y asi sucesivamente, hasta llegar ultimo
        for (i = 0; i < n; i++)
            for (j = 1; j <= buckets.[i].count; j++)
                printf("%d ", buckets[i].stack[j]);
          
        free(arr);
        free(buckets);    
    }
    
    return 0;
}
