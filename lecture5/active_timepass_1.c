#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NUM 3

pthread_cond_t cond;
pthread_mutex_t mutex;
int count = 0;
int task[MAX_NUM];

void *consumer(void *arg)
{
	for (;;)
	{
		int num;
		
		pthread_mutex_lock(&mutex);		
		if (count > 0)
		{
			count--;
			num = task[count];
			printf("Consumer processed item: %d\n", num);
		}
		pthread_mutex_unlock(&mutex);

		if(num == MAX_NUM) break;
	}
	return NULL ;
}

void *producer(void *arg)
{
	unsigned short int xsubi[3] = {3, 7, 11};
	int num = 0;

	for (;;)
	{
		double sleep_time = 1.0 + erand48(xsubi);
		if (num == 2)  usleep(1000000 * sleep_time);

		pthread_mutex_lock(&mutex);
		task[count] = num;
		count++;
		printf("Producer slept for %lf seconds created item: %d\n",sleep_time, num);
		pthread_mutex_unlock(&mutex);

		if(num == MAX_NUM) break;
		num++;
	}
	return NULL ;
}

int main()
{
    pthread_t prod, cons;

    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&cons, NULL, &consumer, NULL);
    pthread_create(&prod, NULL, &producer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

	return 0;
}
