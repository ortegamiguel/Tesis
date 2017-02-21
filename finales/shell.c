#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


int main()
{
    int *arr;
    int i,j,k,tmp,num,I;
    struct timeval t_ini, t_fin, t_1, t_2;
    scanf("%d", &num);
    getchar();
    arr = malloc(num*sizeof(int *));
    for(k=0; k<num; k++)
    {
        scanf("%d",&arr[k]);
        getchar();
    }
    
    gettimeofday(&t_ini, 0);
    for(i=num/2, I=0; i>0; i=i/2, I++)
    {
		gettimeofday(&t_1, 0);
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
		gettimeofday(&t_2, 0);
		printf("\nTiempo iteracion %d (gap_actual=%d) = %f segundos\n", I, i, (t_2.tv_sec -  t_1.tv_sec)+ (float)(t_2.tv_usec - t_1.tv_usec)/(1000000.0));
				
    }
    gettimeofday(&t_fin, 0);
    printf("Tiempo :: %f segundos \n",(t_fin.tv_sec - t_ini.tv_sec)+(float)(t_fin.tv_usec - t_ini.tv_usec)/(1000000.0));
    //printf("%d\n",num );
    //for(k=0; k<num; k++)
        //printf("%d\n",arr[k]);
    free(arr);
    return 0;
}
