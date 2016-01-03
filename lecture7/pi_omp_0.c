#include <stdio.h>
#include <math.h>
#include <omp.h>

#define STEPS 100000

void main()
{
	double step_size = 1.0/STEPS;
	double sum = 0;

	#pragma omp parallel reduction(+:sum)
	{
		double low = 0.5 * step_size +
				(1.0 / omp_get_num_threads()) * omp_get_thread_num();
		double upp = (1.0 / omp_get_num_threads()) * omp_get_thread_num() + 1;

		while(low < upp)
		{
			sum += sqrt(1-low*low) * step_size;
			low += step_size;
		}
	}
	sum *= 4;

	printf("Reference PI = %.10lf Computed PI = %.10lf\n", M_PI, sum);
	printf("Difference to Reference is %.10lf\n", M_PI - sum);
}
