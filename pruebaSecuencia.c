#include <stdio.h>

int main(int argc, char const *argv[])
{
	int num1, num2,i, num;
	scanf("%d", &num);
	getchar();
	scanf("%d", &num1);
	getchar();
	for (i = 0; i < num; i++)
	{
		scanf("%d", &num2);
		if (num2 < num1)
		{
			printf("No Ordena!!!! \n");
			break;
		}
		else{
			printf("%d < %d\n", num2,num1);
			num1 = num2;	
		}
		getchar();
	}

	return 0;
}