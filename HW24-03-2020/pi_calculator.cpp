#include <stdio.h>

int main()

{
	static long num_steps = 100000;
	
	
	double step, pi;
    int i;

    double x, sum = 0.0;

 

    step = 1.0 / (double)num_steps;

    for (i = 0; i < num_steps; i++) {

        x = (i + 0.5) * step;

        sum = sum + 4.0 / (1.0 + x * x);

    }

    pi = step * sum;

    printf("Pi = % f\n", pi);

}
