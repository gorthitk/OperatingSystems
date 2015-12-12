/* Author : Teja Sasank Gorthi
   Email  : jet.sasank@gmail.com */

/**
 *
 * This is a POSIX solution using unnamed semaphores.
 * 
 * This solution will not work on OS X systems, 
 * but will work with Linux.
 * Design a programming solution to the bounded-buffer problem using the producer and consumer processes. 
 * Use standard counting semaphores for empty and full and a mutex lock to represent mutex. 
 *The producer and consumer—running as separate threads—will move items to and from a buffer that is synchronized with the empty, full, and mutex structures.
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
        int return_val = 0;   
	sem_wait(&empty);

	pthread_mutex_lock(&mutex);
	if (insertPointer < BUFFER_SIZE) {
	buffer[insertPointer++] = item;
	insertPointer = insertPointer % 5;
	} else {
	    return_val = -1;
	}
	pthread_mutex_unlock(&mutex);
        sem_post(&full);

	return return_val;	
}

int remove_item(buffer_item *item)
{
	int return_val;
	sem_wait(&full);

	pthread_mutex_lock(&mutex);
	if (insertPointer > 0) {
	*item = buffer[removePointer];
	buffer[removePointer++] = -1;
	removePointer = removePointer % 5;
    } else {
        return_val = -1;
    }
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);

	return return_val;	
}


int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

	/* Initialize the synchronization tools */
	printf("%d\n",pthread_mutex_init(&mutex, NULL));
	printf("%d\n",sem_init(&empty, 0, 5));
	printf("%d\n",sem_init(&full, 0, 0));
	srand(time(0));

	/* Create the producer and consumer threads */
	for(i = 0; i < producerThreads; i++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, producer, NULL);
	}

	for(j = 0; j < consumerThreads; j++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, consumer, NULL);
	}

	/* Sleep for user specified time */
	sleep(sleepTime);
	return 0;
}

void *producer(void *param)
{
	buffer_item random;
	int r;

	while(TRUE)
	{
		r = rand() % 5;
		sleep(r);
		random = rand();

		if(insert_item(random))
			fprintf(stderr, "Error");

		printf("Producer produced %d \n", random);

	}

}

void *consumer(void *param)
{
	buffer_item random;
	int r;

	while(TRUE)
	{
		r = rand() % 5;
		sleep(r);

		if(remove_item(&random))
			fprintf(stderr, "Error Consuming");
		else
			printf("Consumer consumed %d \n", random);
	}
}
