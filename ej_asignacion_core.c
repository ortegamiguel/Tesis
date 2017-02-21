#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <omp.h>
#include <math.h>

double waste_time(long n);

int main(int argc, char *argv[])
{
     int N_THREADS;

     if (argc != 2)
     {
        printf("\nEjecutar como 'a.out Num_threads'\n");
        return 0;
     }
     N_THREADS = atoi(argv[1]);
     omp_set_num_threads(N_THREADS);
     #pragma omp parallel
     {
        int tid = omp_get_thread_num(), procs = omp_get_num_threads();
        unsigned long np, len;
		double res = 0;

        cpu_set_t mascara;

        CPU_ZERO (&mascara);//'limpia' la variable mascara (la coloca en 0)
        CPU_SET (tid, &mascara);//con 'mascara' se indica el numero del thread en donde se ejecutara el thread actual.
	//CPU_SET (0, &mascara);
	
        //A continuacion se establece que el thread actual (el primer parametro '0' indica el thread actual) se ejecute en el core 'mascara'-esimo. 'len' solo indica el largo de la variable 'mascara'. 
        if (sched_setaffinity(0, sizeof(cpu_set_t), &mascara) < 0)
           printf("\nError :: No se pudo hacer la asignacion de un thread a un core\n");

        printf("waste_time()...\n");
        fflush(stdout);
        res = waste_time(3000);
        printf("\nID Thread = %d :: Numero de Threads = %d :: res = %lf\n", tid, procs, res);

        //-------------

        //Lo siguiente asigna todos los threads al primer core. Esto se puede verificar con el comando 'top', en donde el % de CPU usado no deberia superar el 100% (de este mismo modo por ejemplo cuando se ejecuten 4 threads en 4 cores distintos el % de CPU deberia llegar a lo mas a 400% de uso de CPU).
/*
        CPU_SET (0, &mascara);//el thread actual se ejecuta en el core 0
        if (sched_setaffinity(0, len, &mascara) < 0)
           printf("\nError :: No se pudo hacer la asignacion de un thread a un core\n");

        printf("waste_time()...\n");
        fflush(stdout);
        res = waste_time(10000);
        printf("\nID Thread = %d :: Numero de Threads = %d :: res = %.1lf\n", pid, procs, res);
*/        
     }
	return 0;
}

double waste_time(long n)
{
    double res = 0;
    long i = 0;
    while (i <n * 200000) 
    {
       i++;
       res += sqrt((double)i);
    }
   return res;
}


