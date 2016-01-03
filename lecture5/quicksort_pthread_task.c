#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

double time_diff(const struct timespec *first, const struct timespec *second,
		struct timespec *diff)
{
	struct timespec tmp;
	const struct timespec *tmp_ptr;

	if (first->tv_sec > second->tv_sec
			|| (first->tv_sec == second->tv_sec
					&& first->tv_nsec > second->tv_nsec))
	{
		tmp_ptr = first;
		first = second;
		second = tmp_ptr;
	}

	tmp.tv_sec = second->tv_sec - first->tv_sec;
	tmp.tv_nsec = second->tv_nsec - first->tv_nsec;

	if (tmp.tv_nsec < 0)
	{
		tmp.tv_sec -= 1;
		tmp.tv_nsec += 1000000000;
	}

	if (diff != NULL )
	{
		diff->tv_sec = tmp.tv_sec;
		diff->tv_nsec = tmp.tv_nsec;
	}

	return tmp.tv_sec + tmp.tv_nsec / 1000000000.0;
}

int * random_int_array(long size, int num_swaps, unsigned int seed)
{
	srand(seed);

	int *a = malloc(size * (long) sizeof(*a));

	for (int i = 0; i < size; i++)
		a[i] = i;

	for (long i = 0; i < num_swaps; i++)
	{
		long idx0 = rand() % size;
		long idx1 = rand() % size;

		int tmp = a[idx0];
		a[idx0] = a[idx1];
		a[idx1] = tmp;
	}
	return a;
}

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

typedef struct stack_item
{
	int left;
	int right;
} stack_item_t;

#define NUM_THREADS 3

int idle_count = 0;
stack_item_t work_stack[2048];
int stack_size = 0;
int *A;

pthread_mutex_t mtex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condv = PTHREAD_COND_INITIALIZER;

void quicksort_task(int left, int right)
{
	if(left < right)
	{
		int x = left, y = (left+right)/2, z =right;
		int pivotIdx = (A[x] <= A[y])
		    ? ((A[y] <= A[z]) ? y : ((A[x] < A[z]) ? z : x))
		    : ((A[x] <= A[z]) ? x : ((A[y] < A[z]) ? z : y));

		int pivotVal = A[pivotIdx];
		swap(A + pivotIdx, A + right);

		int swapIdx = left;

		for (int i = left; i < right; i++)
		{
			if (A[i] <= pivotVal)
			{
				swap(A + swapIdx, A + i);
				swapIdx++;
			}
		}
		swap(A + swapIdx, A + right);

		pthread_mutex_lock(&mtex);

		if (left < swapIdx - 1)
		{
			work_stack[stack_size].left = left;
			work_stack[stack_size].right = swapIdx - 1;

			stack_size++;
		}

		if (swapIdx + 1 < right)
		{
			work_stack[stack_size].left = swapIdx + 1;
			work_stack[stack_size].right = right;

			stack_size++;
		}

		pthread_cond_broadcast(&condv);

		pthread_mutex_unlock(&mtex);
	}
}

void *qsort_pthread()
{
	stack_item_t local = (stack_item_t) {0,0};
	bool idle = false;

	while(true)
	{
		quicksort_task(local.left, local.right);

		pthread_mutex_lock(&mtex);

		while (stack_size <= 0 && idle_count < NUM_THREADS - 1)
		{
			// Don't increment count if woken up without any reason
			if(idle == false)
			{
				idle = true;
				idle_count++;
			}
			pthread_cond_wait(&condv, &mtex);
		}
		// The last active threads wakes up all threads and exits loop
		if(idle_count == NUM_THREADS - 1 && stack_size <= 0)
		{
			pthread_cond_broadcast(&condv);
			pthread_mutex_unlock(&mtex);
			return NULL;

		}
		// Decrement counter if thread was idle before
		if(idle == true)
		{
			idle = false;
			idle_count--;
		}

		stack_size = stack_size - 1;
		local = work_stack[stack_size];

		pthread_mutex_unlock(&mtex);
	}

	return NULL;
}

void quicksort(int *a, int left, int right)
{
    pthread_t *pthreads;

	pthreads = malloc((unsigned long)NUM_THREADS * sizeof(*pthreads));

	A = a;
	work_stack[stack_size] = (stack_item_t) {left, right };
	stack_size++;

	for (int i = 0; i < NUM_THREADS; i++)
	{
		pthread_create(pthreads + i, NULL, &qsort_pthread, NULL);
	}

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(pthreads[i], NULL );
}

void print_array(int *a, int elements)
{
	for (int i = 0; i < elements; i++)
		printf("%4d ", a[i]);
	printf("\n");
}

void main(int argc, char **argv)
{
	int elements = 1024;
	if (argc > 1)
		elements = atoi(argv[1]);
	struct timespec start, stop;
	int *a = random_int_array(elements, elements / 2, 13);

//	print_array(a, elements);

	clock_gettime(CLOCK_MONOTONIC, &start);

	quicksort(a, 0, elements - 1);

	clock_gettime(CLOCK_MONOTONIC, &stop);

//	print_array(a, elements);

	printf("\nTime = %lf\n", time_diff(&start, &stop, NULL ));
}
