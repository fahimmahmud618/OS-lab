#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int read_counter =0, a=0,b;
void *function_for_read();
void *function_for_write();
sem_t mutex;
sem_t rw_mutex;

int main()
{
	pthread_t threads[20];
	sem_init(&mutex,0,1);
	sem_init(&rw_mutex,0,1);

	int i;

	for(i=0;i<20;i++)
	{
		if(i%2==0)
		pthread_create( &threads[i], NULL, function_for_read, NULL );

		else
		pthread_create( &threads[i], NULL, function_for_write, NULL );
	}

	for(i=0;i<20;i++)
	{
		pthread_join( threads[i], NULL);
	}	

	sem_destroy(&mutex);
	sem_destroy(&rw_mutex);
}

void *function_for_read()
{
  
	sem_wait(&mutex);
	read_counter++;
	
	if (read_counter == 1)
		sem_wait(&rw_mutex);
	sem_post(&mutex);

		b=a; 		/* reading is performed */
		printf("Read B = %d from reader thread\n",b);		

	sem_wait(&mutex);
	read_counter--;
	if (read_counter == 0)
		sem_post(&rw_mutex);
	sem_post(&mutex);
}

void *function_for_write()
{
	sem_wait(&rw_mutex);

	printf("Incrementing a from writer thread\n");
	a++;	/* writing is performed */

	sem_post(&rw_mutex);
}
