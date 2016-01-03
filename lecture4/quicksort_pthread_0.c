#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

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

typedef struct pthread_args
{
	int *a;
	int left;
	int right;
} pthread_args_t;

void *qsort_pthread(void *ptr_arg)
{
	int *a = ((pthread_args_t*) ptr_arg)->a;
	int left = ((pthread_args_t*) ptr_arg)->left;
	int right = ((pthread_args_t*) ptr_arg)->right;

	if (left < right)
	{
		int x = left, y = (left+right)/2, z =right;
		int pivotIdx = (a[x] <= a[y])
		    ? ((a[y] <= a[z]) ? y : ((a[x] < a[z]) ? z : x))
		    : ((a[x] <= a[z]) ? x : ((a[y] < a[z]) ? z : y));

		int pivotVal = a[pivotIdx];
		swap(a + pivotIdx, a + right);

		int swapIdx = left;

		for (int i = left; i < right; i++)
		{
			if (a[i] <= pivotVal)
			{
				swap(a + swapIdx, a + i);
				swapIdx++;
			}
		}
		swap(a + swapIdx, a + right);

		pthread_t left_pthread, right_pthread;
		pthread_args_t l_pthread_args, r_pthread_args;

		l_pthread_args = (pthread_args_t){ a, left, swapIdx -1 };
		pthread_create(&left_pthread, NULL, &qsort_pthread, &l_pthread_args);

		r_pthread_args = (pthread_args_t){ a, swapIdx + 1, right };
		pthread_create(&right_pthread, NULL, &qsort_pthread, &r_pthread_args);

		pthread_join(left_pthread, NULL);
		pthread_join(right_pthread, NULL);
	}

	return NULL;
}

void quicksort(int *a, int left, int right)
{
    pthread_t first_pthread;
    pthread_args_t f_pthread_args;

    f_pthread_args = (pthread_args_t){ a, left, right };
    pthread_create(&first_pthread, NULL, &qsort_pthread, &f_pthread_args);
    pthread_join(first_pthread, NULL);
}

void print_array(int *a, int elements)
{
	for (int i = 0; i < elements; i++)
		printf("%4d ", a[i]);
	printf("\n");
}

void main(int argc, char **argv)
{
	int elements = 27;
	if (argc > 1)
		elements = atoi(argv[1]);
	struct timespec start, stop;
	int *a = random_int_array(elements, elements / 2, 13);

	print_array(a, elements);

	clock_gettime(CLOCK_MONOTONIC, &start);
	quicksort(a, 0, elements - 1);
	clock_gettime(CLOCK_MONOTONIC, &stop);

	print_array(a, elements);

	printf("\nTime = %lf\n", time_diff(&start, &stop, NULL ));
}
