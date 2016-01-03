#include <stdio.h>
#include <math.h>

#define STEPS 100000

void main()
{
	double step_size = 1.0/STEPS;
	double sum = 0;

	#pragma omp parallel for reduction(+:sum)
	for(int i=0; i < STEPS; i++)
	{
		sum += sqrt(1-(i*step_size)*(i*step_size)) * step_size;
	}
	sum *= 4;

	printf("Reference PI = %.10lf Computed PI = %.10lf\n", M_PI, sum);
	printf("Difference to Reference is %.10lf\n", M_PI - sum);
}
