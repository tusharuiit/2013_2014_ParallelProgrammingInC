#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STEPS 1000000

int main()
{
	double step_size = 1.0/STEPS , t = 0.5 * step_size ; double sum = 0 ;
	while(t < 1.0)
	{
		sum += sqrt(1.0-t*t) * step_size;
		t += step_size;
	}
    sum*=4.0 ;	

	printf("Reference PI = %.10lf Computed PI = %.10lf\n", M_PI, sum);
	printf("Difference to Reference is %.10lf\n", M_PI - sum);
}
