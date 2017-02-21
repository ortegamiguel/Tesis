#include <stdio.h>
#include <stdlib.h>
#define MAX 6
#define TAM 999
#define N 10
#define SHOWPASS

void print(int *a, int n)
{
  int i;
  for (i = 0; i < n; i++)
    printf("%d\t", a[i]);
}

void radixsort(int *a, int n)
{
  int i, b[MAX], m = 0, exp = 1, j=0;

  for (i = 0; i < n; i++)
  {
    if (a[i] > m)  //determinamos cual es mayor
      m = a[i];
  }

  while (m / exp > 0)
  {
    int bucket[N] = {0};

    for (i = 0; i < n; i++)
      bucket[a[i] / exp % 10]++;

    for (i = 1; i < N; i++)
      bucket[i] += bucket[i - 1];

    for (i = n - 1; i >= 0; i--)
    {
      b[--bucket[a[i] / exp % 10]] = a[i];
    }

    for (i = 0; i < n; i++)
      a[i] = b[i];

    #ifdef SHOWPASS
      j++;
      printf("\nItera %d  : ",j);
      print(a, n);
      printf("%d",exp);
    #endif

    exp *= 10; //incremento de 1 10 100 1000 etc
  }
}


int main()
{
  int arr[MAX];
  int i, n=MAX;
  randomize();
  clrscr();

  for (i = 0; i < n; i++)
    arr[i]=rand()%TAM;

  printf("\nOriginal : ");
  print(&arr[0], n);
  printf("Evaluacion");

  radixsort(&arr[0], n);

  printf("\nOrdenados: ");
  print(&arr[0], n);
  printf("\n");
  getch();
  return 0;
}