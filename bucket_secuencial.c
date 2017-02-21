#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct Bucket {
	int* Data;
	int Count;
};

int* AddItemInArray(int *arr, int count, int item) {
	int* newArr = malloc(sizeof(int) * (count + 1));
  int i;
	for (i = 0; i < count; i++) {
		  newArr[i] = arr[i];
	}

	newArr[count] = item;

	return newArr;
}

void BucketSort(int *data, int count) {
    int i,j;
  	int minValue = data[0];
  	int maxValue = data[0];
		//maximo y minimo valor
  	for (i = 1; i < count; i++)
  	{
    		if (data[i] > maxValue)
    			 maxValue = data[i];
    		if (data[i] < minValue)
    			 minValue = data[i];
  	}
		
  	int bucketLength = maxValue - minValue + 1;
  	struct Bucket* bucket = malloc(sizeof(struct Bucket) * bucketLength);

  	for (i = 0; i < bucketLength; i++)
  	{
  		  bucket[i] = *(struct Bucket*)malloc(sizeof(struct Bucket));
  	}

  	for (i = 0; i < count; i++)
  	{
    		struct Bucket b = bucket[data[i] - minValue];
    		if (b.Count < 0) {
      			b.Count = 0;
    		}

    		bucket[data[i] - minValue].Data = AddItemInArray(b.Data, b.Count++, data[i]);
    		bucket[data[i] - minValue].Count = b.Count;
  	}

  	int k = 0;
  	for (i = 0; i < bucketLength; i++)
  	{
  		int bucketSize = bucket[i].Count;

  		if (bucketSize > 0)
  		{
    			for (j = 0; j < bucketSize; j++)
    			{
      				data[k] = bucket[i].Data[j];
      				k++;
    			}
  		}
  	}
}

int main()
{
    int *arr, num, i;
    struct timeval t_ini, t_fin;
    scanf("%d",&num);
    arr = (int *)calloc(num,sizeof(int ));
    for(i = 0; i < num; i++ )
    {
        scanf("%d",&arr[i]);
    }

    gettimeofday(&t_ini, 0);
    BucketSort(arr, num);
    gettimeofday(&t_fin, 0);
	  printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);


    return 0;
}
