#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    int *arr;
    int i,j,k,tmp,num;
    time_t start, end;
    scanf("%d", &num);
    getchar();
    arr = malloc(num*sizeof(int *));
    for(k=0; k<num; k++)
    {
        scanf("%d",&arr[k]);
        getchar();
    }
    
    start = time(NULL);
    for(i=num/2; i>0; i=i/2)
    {
        for(j=i; j<num; j++)
        {
            for(k=j-i; k>=0; k=k-i)
            {
                //  printf("arr[%d] - arr[%d]\n", k, k+i);
                if(arr[k+i]>=arr[k])
                {
                    //printf("apspa\n");
                    break;
                }
                else
                {
                    tmp=arr[k];
                    arr[k]=arr[k+i];
                    arr[k+i]=tmp;
                }
            }
        }
    }
    end = time(NULL);
    printf("Tiempo :: %f  \n",  difftime(end, start));
    //printf("%d\n",num );
    //for(k=0; k<num; k++)
        //printf("%d\n",arr[k]);
    free(arr);
    return 0;
}