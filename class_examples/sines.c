#include <stdio.h>
#include <math.h>

int main(void)
{
	int angle_degree;
	double angle_radian, value;

	printf("\nCompute a table of the sine function\n\n");
	printf("Angle\tSine\n");
	angle_degree = 0;
	while (angle_degree <= 360) {
		angle_radian = M_PI * angle_degree / 180.0;
		value = sin(angle_radian);
		printf("%3d\t%f\n", angle_degree, value);
		angle_degree += 10;
	}
	return 0;
}
