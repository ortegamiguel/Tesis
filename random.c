#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int main(){
	int a,b;
	a = -100;
	b = 0;
	srand(time(NULL));
	printf("%d", rand()%a+b);
}
