#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define STEPS 1000000000
#define CHUNK_SIZE 10
#define STEP_SIZE 1.0/STEPS

struct pthread_args
{
	int iterations;
	pthread_mutex_t mutex;
};

void * pi_thread(void *ptr)
{
	double tsum = 0; struct pthread_args *args = (struct pthread_args*)ptr;

	for(;;)
	{
		pthread_mutex_lock(&(args->mutex));
		int i = args->iterations;
		args->iterations += CHUNK_SIZE;
		pthread_mutex_unlock(&(args->mutex));

		if(i + CHUNK_SIZE > STEPS)
			break;

		double low = 0.5 * STEP_SIZE + STEP_SIZE * i;
		double upp = STEP_SIZE * (i + CHUNK_SIZE);

		while(low < upp)
		{
			tsum += sqrt(1-low*low) * STEP_SIZE;
			low += STEP_SIZE;
		}
	}

	double *local_sum = malloc(sizeof(*local_sum));
	*local_sum = tsum;

	return local_sum;
}

void main()
{
	long num_threads = 4; double sum = 0; pthread_t *thread; void *lsum;
	struct pthread_args thread_arg = {0, PTHREAD_MUTEX_INITIALIZER};

	thread = malloc((unsigned long)num_threads * sizeof(*thread));

	for (int i = 0; i < num_threads; i++)
	{
		pthread_create(thread + i, NULL, &pi_thread, &thread_arg);
	}

	for (int i = 0; i < num_threads; i++)
	{
		pthread_join(thread[i], &lsum );
		sum += 4 * (*(double*)lsum);
		free(lsum);
	}

	printf("Reference PI = %.10lf Computed PI = %.10lf\n", M_PI, sum);
	printf("Difference to Reference is %.10lf\n", M_PI - sum);
}
