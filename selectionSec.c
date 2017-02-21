#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int *arr, num,q, i, j, min,tmp;
	struct timeval t_ini, t_fin;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }
    gettimeofday(&t_ini, 0);
    for (i = 0; i < num; i++)
    {
    	min = i;
    	for (j = i+1; j < num; j++)
    		if (arr[j] < arr[min])
    			min = j;
    	
    	tmp = arr[i];
    	arr[i] = arr[min];
    	arr[min] = tmp;
    }
    gettimeofday(&t_fin, 0);
	printf("Tiempo :: %f  segundos\n\n", (t_fin.tv_sec - t_ini.tv_sec) + (float)(t_fin.tv_usec - t_ini.tv_usec)/1000000.0);


	return 0;
}