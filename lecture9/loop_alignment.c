// compile with: gcc -std=c99 -fopenmp -o loop_alignment loop_alignment.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define N 30

void unaligned(int *a, int *b, int *d, int n)
{
	for(int i=0; i < n; i++)
	{
		b[i] = d[i];
		a[i] = a[i] + b[i + 1];
	}
}

void aligned_right_shift(int *a, int *b, int *d, int n)
{
	b[0] = d[0];

	#pragma omp parallel for
	for(int i=0; i < n-1; i++)
	{
		a[i] = a[i] + b[i + 1];
		b[i + 1] = d[i + 1];
	}

	a[n-1] = a[n-1] + b[n-1 + 1];
}

void aligned_left_shift(int *a, int *b, int *d, int n)
{
	b[0] = d[0];
	
	#pragma omp parallel for
	for(int i=1; i < n; i++)
	{
		a[i-1] = a[i-1] + b[i];
		b[i] = d[i];
	}

	a[n-1] = a[n-1] + b[n-1 + 1];
}

void print_array(int *array, int n)
{
	for(int i=0; i<n; i++)
	{
		printf("%d2 ", array[i]);
	}
	printf("\n");
}

int main ()
{
	int a0[N+1];
	int b0[N+1];
	int d0[N+1];

	int a1[N+1];
	int b1[N+1];
	int d1[N+1];

	int a2[N+1];
	int b2[N+1];
	int d2[N+1];

	int random;

	/* initialize random seed: */
	srand ( time(NULL) );

	for(int i=0; i<N+1; i++)
	{
		random = rand() % 10;
		a0[i] = random;
		a1[i] = random;
		a2[i] = random;

		random = rand() % 10;
		b0[i] = random;
		b1[i] = random;
		b2[i] = random;

		random = rand() % 10;
		d0[i] = random;
		d1[i] = random;
		d2[i] = random;
	}

	unaligned(a0, b0, d0, N);
	aligned_right_shift(a1, b1, d1, N);
	aligned_left_shift(a2, b2, d2, N);

	printf("a0, a1, a2\n");
	print_array(a0, N+1);
	print_array(a1, N+1);
	print_array(a2, N+1);

	printf("\nb0, b1, b2\n");
	print_array(b0, N+1);
	print_array(b1, N+1);
	print_array(b2, N+1);

	printf("\nd0, d1, d2\n");
	print_array(d0, N+1);
	print_array(d1, N+1);
	print_array(d2, N+1);

}
