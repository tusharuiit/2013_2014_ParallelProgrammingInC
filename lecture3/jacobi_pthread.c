#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define EPSILON 0.00001
#define MAX_ITER 1000000

void init(int n, double (**A)[n], double **b, double **x0, double **x1)
{
	*A = malloc(n * n * sizeof(double));
	*b = malloc(n * sizeof(double));
	*x0 = malloc(n * sizeof(double));
	*x1 = malloc(n * sizeof(double));

	unsigned short xi[3] = {4, 153, 17};
	int i,j;

	for (i = 0; i < n; i++) {
	  (*A)[i][i] = 0.0;
	  (*x0)[i] = 0.0;
	  (*x1)[i] = 0.0;
	  for (j = 0; j < n; j++) {
		 if (i != j)
		 {
			 (*A)[i][j] = erand48(xi);
			 (*A)[i][i] += 2.0 * (*A)[i][j];
		 }
	  }
	  (*b)[i] = 0.0;
	  for (j = 0; j < n; j++) {
		  (*b)[i] += (*A)[i][j] * j;
	  }
	}
}

void print_vector(int n, double v[n])
{
	for(int i = 0; i < n; i++)
		printf("%lf ", v[i]);
	printf("\n");
}

void print_matrix(int n, double M[n][n])
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%lf ", M[i][j]);
		}
		printf("\n");
	}
}

struct pthread_args
{
	int id;
	int tnum;
	int n;
	int *iter;

	double *stop;
	double *A;
	double *b;
	double *x0;
	double *x1;

	pthread_mutex_t *mutex;
	pthread_barrier_t *barrier;
};

void * pthread_jacobi(void *args)
{
	struct pthread_args *a = (struct pthread_args*)args;

	int id = a->id, n = a->n, iter = 0, tnum = a->tnum;
	double l_stop, *stop = a->stop, *tmp,
			(*A)[n] = a->A, *b = a->b, *x0 = a->x0, *x1 = a->x1;
	do
	{
		l_stop = 0.0;
		for (int i = id*(n/tnum); i < (id+1)*(n/tnum); i++)
		{
		  x1[i] = 0.0;
		  for (int j = 0; j < n; j++)
		  {
			  if(i != j)
				  x1[i] += A[i][j] * x0[j];
		  }
		  x1[i] = (b[i] - x1[i]) / A[i][i];
		  l_stop += pow(x1[i] - x0[i], 2.0);
		}

		tmp = x1; x1 = x0; x0 = tmp;
		iter++;

		pthread_barrier_wait(a->barrier);
		if(id == 0) *stop = 0.0;
		pthread_barrier_wait(a->barrier);

		pthread_mutex_lock(a->mutex);
		*stop += l_stop;
		pthread_mutex_unlock(a->mutex);

		pthread_barrier_wait(a->barrier);
	} while((sqrt(*stop) > EPSILON) && (iter < MAX_ITER));

	if(id == 0) *(a->iter) = iter;

	return NULL;
}

void main(int argc, char **argv)
{
	int n = 14, iter = 0, tnum = 2;
	if(argc > 2)
	{	n = atoi(argv[1]); tnum = atoi(argv[1]); }

	double stop, (*A)[n], *b, *x0, *x1;

	pthread_mutex_t mutex;
	pthread_barrier_t barrier;

	pthread_barrier_init(&barrier, NULL, tnum);
	pthread_mutex_init(&mutex, NULL);

	init(n, &A, &b, &x0, &x1);

	printf("Matrix A:\n");
	print_matrix(n, A);
	printf("\nVector b:\n");
	print_vector(n, b);
	printf("\nInitial Guess of Vector x:\n");
	print_vector(n, x1);

	pthread_t *thread; struct pthread_args *thread_arg;

	thread = malloc((unsigned long)tnum * sizeof(*thread));
	thread_arg = malloc((unsigned long)tnum * sizeof(*thread_arg));

	for (int i = 0; i < tnum; i++)
	{
		thread_arg[i] =
	   (struct pthread_args){ i, tnum, n, &iter, &stop, A, b, x0, x1, &mutex, &barrier};
		pthread_create(thread + i, NULL, &pthread_jacobi, thread_arg + i);
	}

	for (int i = 0; i < tnum; i++)
	{
		pthread_join(thread[i], NULL );
	}

	printf("\nSolution Vector x after %d Iterations:\n", iter);
	print_vector(n, x1);
}
