#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	int *arr, num, q, i;
	scanf("%d",&num);
	getchar();
    arr = malloc(num*sizeof(int *));
    for(q=0; q<num; q++)
    {
       	scanf("%d",&arr[q]);
       	getchar();
    }

    for (i = 0; i < num; i++)
    {
    	if (arr[i] > arr[i+1])
    	{
    		printf("NUNCA ORDENO BIEN\n");
    		printf("%d\n", arr[i]);
    		printf("%d\n", arr[i+1]);
    		break;
    	}
    }

	return 0;
}