#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;
void *functionC(void *arg);
#define size 9000
int matrix[size][size] ;

main()
{
	double exectime;
	struct timeval tstart, tend;
	int a,b;
	for(a=0;a<size;a++)
	{
		for(b=0;b<size;b++)
		{
			matrix[a][b]=1;
		}
	}
   int i;
   pthread_t threads[size];

   /* Create independent threads each of which will execute functionC */
	gettimeofday( &tstart, NULL );
   for ( i = 0; i < size; ++i ) {
    int *tid;
    tid = (int *) malloc( sizeof(int) );
    *tid = i;
    pthread_create( &threads[i], NULL, functionC, (void *)tid );
  }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */

   for ( i = 0; i < size; ++i )
{
	pthread_join( threads[i], NULL);
}
   gettimeofday( &tend, NULL );
   

   printf("Counter value: %d",counter);

  exectime = (tend.tv_sec - tstart.tv_sec) * 1000.0; // sec to ms
  exectime += (tend.tv_usec - tstart.tv_usec) / 1000.0; // us to ms   

  printf( "\tExecution time:%.3lf sec\n", exectime/1000.0);
   exit(0);
}

void *functionC(void *arg)
{
  
   int i=0;
   int row = *(int *)(arg);
   //pthread_mutex_lock( &mutex1 );
   for(i = 0; i<size;i++)
   {
	if(matrix[row][i]==1)
		counter++;

   }
   
   //printf("Counter value: %d\n",counter);
   //pthread_mutex_unlock( &mutex1 );
}



