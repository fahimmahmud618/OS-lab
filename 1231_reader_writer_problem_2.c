#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int read_counter =0, write_counter =0,a=0,b;
int num_writer=0, num_reader=0;
void *function_for_read();
void *function_for_write();
sem_t read_counter_mutex;\
sem_t write_counter_mutex;
sem_t read_try;
sem_t resource;

int main()
{
	pthread_t threads[20];
	sem_init(&read_counter_mutex,0,1);
	sem_init(&write_counter_mutex,0,1);
	sem_init(&read_try,0,1);
	sem_init(&resource,0,1);

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

	sem_destroy(&read_counter_mutex);
	sem_destroy(&write_counter_mutex);
	sem_destroy(&read_try);
	sem_destroy(&resource);
}

void *function_for_read()
{
	num_reader++;
 //entry 
	sem_wait(&read_try);
	sem_wait(&read_counter_mutex);
	read_counter++;
	if (read_counter == 1)
		sem_wait(&resource);
	sem_post(&read_counter_mutex);
	sem_post(&read_try);

//critical
		//b=a; 		/* reading is performed */
		printf("Read:: a = %d from reader no %d\n",a,num_reader);	


//exit
	sem_wait(&read_counter_mutex);
	read_counter--;
	if (read_counter == 0)
		sem_post(&resource);
	sem_post(&read_counter_mutex);
	printf("---reader exit---\n");
}

void *function_for_write()
{
	num_writer++;
	//entry

	sem_wait(&write_counter_mutex);
	write_counter++;
	if(write_counter==1)
		sem_wait(&read_try);
	sem_post(&write_counter_mutex);
	sem_wait(&resource);

	//critical
	printf("Write:: Incrementing a from writer no %d\n",num_writer);
	a++;	/* writing is performed */
	sem_post(&resource);

	//exit
	sem_wait(&write_counter_mutex);
	write_counter--;
	if(write_counter==0)
		sem_post(&read_try);
	sem_post(&write_counter_mutex);
	printf("---writer exit---\n");
}
